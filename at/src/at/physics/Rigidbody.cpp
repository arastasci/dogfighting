#pragma once

#include "Rigidbody.h"
#include "PhysicsWorld.h"
#include "at/ecs/CoreComponents/MeshRenderer.h"
#include "CollisionShapeLibrary.h"

namespace at
{
	Rigidbody::Rigidbody(bool isKinematic)
		: m_isStatic(isKinematic)
	{
		
		m_ShiftedCompoundShape = new btCompoundShape();
		m_MotionState = new btDefaultMotionState();
	}

	void Rigidbody::AddBodyToWorld(const SharedPtr<PhysicsWorld>& world)
	{
		m_World = std::weak_ptr(world);


		if (m_Entity.HasComponent<MeshRenderer>())
		{
			auto& mr = m_Entity.GetComponent<MeshRenderer>();
			const auto& name = mr.Model->GetName();
			m_CollisionShape =  CollisionShapeLibrary::Get().CreateOrGetCollisionShape(name, m_isStatic);
							
		}
		else
		{
			assert(false);
		}

		if (!m_isStatic)
		{
			btVector3 inertia;
			btTransform principal;
			m_CollisionShape->CalculatePrincipalAxisTransform(principal, inertia);

			auto* shape = m_CollisionShape->GetShape();

			int numChildShapes = shape->getNumChildShapes();

			for (int i = 0; i < numChildShapes; i++)
			{
				auto adjusted = shape->getChildTransform(i);
				adjusted.setOrigin(adjusted.getOrigin() - principal.getOrigin());
				m_ShiftedCompoundShape->addChildShape(adjusted, shape->getChildShape(i));
			}

			m_Rigidbody = std::make_shared<btRigidBody>(1, m_MotionState, m_ShiftedCompoundShape);
			m_Rigidbody->setActivationState(DISABLE_DEACTIVATION);
			m_Rigidbody->setCollisionShape(m_ShiftedCompoundShape);
			float mass = 50.f;
			m_ShiftedCompoundShape->calculateLocalInertia(mass, inertia);
			m_Rigidbody->setMassProps(mass, inertia);
			m_Rigidbody->updateInertiaTensor();
		}
		else
		{
			m_Rigidbody = std::make_shared<btRigidBody>(0, m_MotionState, m_CollisionShape->GetShape());
		}

		

		btTransform transform = m_Rigidbody->getCenterOfMassTransform();
		auto t = m_Entity.GetComponent<Transform>().GetWorldTransform();
		vec3 p, scale;
		quat rot;
		DecomposeTransform(t, p, rot, scale);
		transform.setOrigin(toBt(p));
		transform.setBasis(toBt(glm::mat3_cast(rot)));
		//m_UniformScalingShape = new btUniformScalingShape(m_CollisionShape->GetShape(), scale.r);
		if (!m_isStatic)
		{
			m_ShiftedCompoundShape->setLocalScaling(toBt(scale));
		}
		else
		{
			m_CollisionShape->GetShape()->setLocalScaling(toBt(scale));

		}
		world->AddRigidbody(m_Rigidbody.get());
		world->UpdateAABB(m_Rigidbody.get());
		m_Rigidbody->setCenterOfMassTransform(transform);
		m_Rigidbody->setUserPointer(this);
	}

	void Rigidbody::GetCollidingObjects(std::vector<Rigidbody*>& colliders)
	{
		m_World.lock()->GetCollidingObjects(colliders, this);
	}

	void Rigidbody::UpdateLastTransform()
	{
		m_Rigidbody->getMotionState()->getWorldTransform(m_LastTransform);
		m_AccTime = 0;
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

		return Transform(toGlm(resPos), toGlm(resQ), vec3(1.0f));
	}

	// TODO: WARNING! these will drop the parent for now, fix it later
	Transform Rigidbody::GetWorldTransform() const
	{
		btTransform btT;
		
		m_Rigidbody->getMotionState()->getWorldTransform(btT);
		auto btQ = btT.getRotation();
		glm::quat q(btQ.w(), btQ.x(), btQ.y(), btQ.z());
		return Transform(toGlm(btT.getOrigin()), q, toGlm(m_ShiftedCompoundShape->getLocalScaling()));
	}


	Transform Rigidbody::GetStaticWorldTransform() const
	{
		btTransform btT = m_Rigidbody->getWorldTransform();
		auto btQ = btT.getRotation();
		glm::quat q(btQ.w(), btQ.x(), btQ.y(), btQ.z());
		return Transform(toGlm(btT.getOrigin()), q, toGlm(m_CollisionShape->GetShape()->getLocalScaling()));
	}

	bool Rigidbody::IsActive()
	{
		return m_isActive;
	}

	void Rigidbody::SetIsActive(bool b)
	{
		m_isActive = b;
	}

	void Rigidbody::DrawShape()
	{
		
	}

	void Rigidbody::ApplyForce(vec3 direction, float force)
	{
		if (!m_World.lock()->GetIsSimulated())
			return;
		//m_Rigidbody->applyCentralForce(toBt(force * direction));
		m_Rigidbody->applyCentralImpulse(toBt(force * direction));	
	}

	void Rigidbody::SetGravity(vec3 acceleration)
	{
		m_Rigidbody->setGravity(toBt(acceleration));
	}

}