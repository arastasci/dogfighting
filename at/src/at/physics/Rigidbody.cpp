#pragma once

#include "Rigidbody.h"
#include "PhysicsWorld.h"
#include "at/ecs/CoreComponents/MeshRenderer.h"
#include "CollisionShapeLibrary.h"


namespace at
{
   
    Rigidbody::Rigidbody(bool isKinematic, bool overrideShape)
        : m_isStatic(isKinematic), m_Override(overrideShape),
        m_ShiftedCompoundShape(std::make_unique<btCompoundShape>()),
        m_MotionState(std::make_unique<btDefaultMotionState>())
    {
    }

    void Rigidbody::Detach()
    {
        if (auto world = m_World.lock())
        {
            if (m_Rigidbody && m_Rigidbody->isInWorld())
            {
                world->RemoveRigidbody(m_Rigidbody.get());
            }
        }
        m_Rigidbody.reset();
    }

    Rigidbody::~Rigidbody()
    {
        Detach();
    }

    Rigidbody::Rigidbody(Rigidbody&& other) noexcept
        :
        m_Owner(other.m_Owner),
        m_isStatic(other.m_isStatic),
        m_isActive(other.m_isActive),
        m_Override(other.m_Override),
        m_LastTransform(other.m_LastTransform),
        m_AccTime(other.m_AccTime),
        m_OverriddenScale(other.m_OverriddenScale),
        m_ShiftedCompoundShape(std::move(other.m_ShiftedCompoundShape)),
        m_MotionState(std::move(other.m_MotionState)),
        m_Rigidbody(std::move(other.m_Rigidbody)),
        m_World(std::move(other.m_World)),
        m_CollisionShape(std::move(other.m_CollisionShape)),
        m_CurrentCollidedObjects(std::move(other.m_CurrentCollidedObjects))
    {

        if (m_Rigidbody)
            m_Rigidbody->setUserPointer(this);
    }

    Rigidbody& Rigidbody::operator=(Rigidbody&& other) noexcept
    {
        if (this != &other)
        {
            Detach();
            m_Owner = other.m_Owner;
            m_isStatic = other.m_isStatic;
            m_isActive = other.m_isActive;
            m_Override = other.m_Override;
            m_LastTransform = other.m_LastTransform;
            m_AccTime = other.m_AccTime;
            m_OverriddenScale = other.m_OverriddenScale;
            m_ShiftedCompoundShape = std::move(other.m_ShiftedCompoundShape);
            m_MotionState = std::move(other.m_MotionState);
            m_Rigidbody = std::move(other.m_Rigidbody);
            m_World = std::move(other.m_World);
            m_CollisionShape = std::move(other.m_CollisionShape);
            m_CurrentCollidedObjects = std::move(other.m_CurrentCollidedObjects);

            if (m_Rigidbody)
                m_Rigidbody->setUserPointer(this);
        }
        return *this;
    }

    void Rigidbody::AddBodyToWorld(const SharedPtr<PhysicsWorld>& world, Entity thisEntity)
    {
        m_World = world;

        if (!m_Override && thisEntity.HasComponent<MeshRenderer>())
        {
            auto& mr = thisEntity.GetComponent<MeshRenderer>();
            const auto& name = mr.Model->GetName();
            m_CollisionShape = CollisionShapeLibrary::Get().CreateOrGetCollisionShape(name, m_isStatic);
        }

        if (m_Override)
        {
            m_ShiftedCompoundShape->addChildShape(btTransform::getIdentity(), new btBoxShape(btVector3(1, 1, 1)));
            m_Rigidbody = std::make_shared<btRigidBody>(1, m_MotionState.get(), m_ShiftedCompoundShape.get());
            m_Rigidbody->setActivationState(DISABLE_DEACTIVATION);
            float mass = 1.f;
            btVector3 inertia;
            m_ShiftedCompoundShape->calculateLocalInertia(mass, inertia);
            m_Rigidbody->setMassProps(mass, inertia);
            m_Rigidbody->updateInertiaTensor();
        }
        else if (!m_isStatic)
        {
            btVector3 inertia;
            btTransform principal;
            m_CollisionShape->CalculatePrincipalAxisTransform(principal, inertia);

            auto* shape = m_CollisionShape->GetShape();
            int numChildShapes = shape->getNumChildShapes();

            for (int i = 0; i < numChildShapes; ++i)
            {
                auto adjusted = shape->getChildTransform(i);
                adjusted.setOrigin(adjusted.getOrigin() - principal.getOrigin());

                auto* originalChild = shape->getChildShape(i);

                m_ShiftedCompoundShape->addChildShape(adjusted, originalChild);
            }

            m_Rigidbody = std::make_shared<btRigidBody>(1, m_MotionState.get(), m_ShiftedCompoundShape.get());
            m_Rigidbody->setActivationState(DISABLE_DEACTIVATION);
            float mass = 50.f;
            m_ShiftedCompoundShape->calculateLocalInertia(mass, inertia);
            m_Rigidbody->setMassProps(mass, inertia);
            m_Rigidbody->updateInertiaTensor();
        }
        else
        {
            m_Rigidbody = std::make_shared<btRigidBody>(0, m_MotionState.get(), m_CollisionShape->GetShape());
        }

        auto t = thisEntity.GetComponent<Transform>().GetWorldTransform();
        vec3 p, s;
        quat r;
        DecomposeTransform(t, p, r, s);

        btTransform transform;
        transform.setOrigin(toBt(p));
        transform.setBasis(toBt(glm::mat3_cast(r)));
        m_LastTransform = transform;
        m_Rigidbody->getCollisionShape()->setLocalScaling(toBt(s));
    
        world->AddRigidbody(m_Rigidbody.get());
        world->UpdateAABB(m_Rigidbody.get());
        m_Rigidbody->setCenterOfMassTransform(transform);
        m_Rigidbody->setUserPointer(this);
        m_Owner = (entt::entity)thisEntity;
    }

    void Rigidbody::GetCollidingObjects(std::vector<Rigidbody*>& colliders)
    {
        if (auto w = m_World.lock())
            w->GetCollidingObjects(colliders, this);
    }

    void Rigidbody::UpdateLastTransform()
    {
        m_Rigidbody->getMotionState()->getWorldTransform(m_LastTransform);
        m_AccTime = 0.0;
    }

    void Rigidbody::ClearCollidedObjects()
    {
        m_CurrentCollidedObjects.clear();
    }

    void Rigidbody::ReportCollidedObject(Rigidbody* body)
    {
        m_CurrentCollidedObjects.insert(body);
    }

    Transform Rigidbody::GetInterpolatedTransform(float dt)
    {
        float t = dt;
        btTransform btT;
        m_Rigidbody->getMotionState()->getWorldTransform(btT);
        btQuaternion currentQ;
        btT.getBasis().getRotation(currentQ);
        btQuaternion lastQ;
        m_LastTransform.getBasis().getRotation(lastQ);
        auto resQ = slerp(lastQ, currentQ, t);

        auto currentPos = btT.getOrigin();
        auto lastPos = m_LastTransform.getOrigin();
        auto resPos = lerp(lastPos, currentPos, t);

        return Transform(Math::toGlm(resPos), Math::toGlm(resQ), vec3(1.0f));
    }

    Transform Rigidbody::GetWorldTransform() const
    {
        btTransform btT;
        m_Rigidbody->getMotionState()->getWorldTransform(btT);
        auto btQ = btT.getRotation();
        glm::quat q(btQ.w(), btQ.x(), btQ.y(), btQ.z());
        return Transform(Math::toGlm(btT.getOrigin()), q, Math::toGlm(m_ShiftedCompoundShape->getLocalScaling()));
    }

    Transform Rigidbody::GetStaticWorldTransform() const
    {
        btTransform btT = m_Rigidbody->getWorldTransform();
        auto btQ = btT.getRotation();
        glm::quat q(btQ.w(), btQ.x(), btQ.y(), btQ.z());
        return Transform(Math::toGlm(btT.getOrigin()), q, Math::toGlm(m_CollisionShape->GetShape()->getLocalScaling()));
    }

    bool Rigidbody::IsActive() const
    {
        return m_isActive;
    }

    void Rigidbody::SetIsActive(bool b)
    {
        m_isActive = b;
    }

    void Rigidbody::SetColliderScale(const btVector3& scale)
    {
        m_OverriddenScale = scale;
    }

    void Rigidbody::ApplyForce(vec3 direction, float force)
    {
        if (auto w = m_World.lock())
        {
            if (!w->GetIsSimulated())
                return;
        }
        m_Rigidbody->applyCentralImpulse(toBt(force * direction));
    }

    void Rigidbody::SetGravity(vec3 acceleration)
    {
        m_Rigidbody->setGravity(toBt(acceleration));
    }
    entt::entity Rigidbody::GetOwner()
    {
        return m_Owner;
    }
}
