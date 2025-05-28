#pragma once

#include "atpch.h"
#include "at/ecs/Entity.h"
#include "at/core/Core.h"
#include "at/utils/Helpers.h"
#include "at/utils/Math.h"
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <BulletCollision/CollisionShapes/btCompoundShape.h>
#include "at/ecs/CoreComponents/Transform.h"

#include "CollisionShape.h"

namespace at
{
    class PhysicsWorld;

    class AT_API Rigidbody 
    {
    public:
        Rigidbody(bool isKinematic = false, bool overrideShape = false);
        ~Rigidbody();

        Rigidbody(const Rigidbody&) = delete;
        Rigidbody& operator=(const Rigidbody&) = delete;
        Rigidbody(Rigidbody&&) noexcept;
        Rigidbody& operator=(Rigidbody&&) noexcept;

        void AddBodyToWorld(const SharedPtr<PhysicsWorld>& world, Entity thisEntity);
        void GetCollidingObjects(std::vector<Rigidbody*>& colliders);
        void UpdateLastTransform();
        void ClearCollidedObjects();
        void ReportCollidedObject(Rigidbody* rb);
        Transform GetInterpolatedTransform(float t);
        Transform GetWorldTransform() const;
        Transform GetStaticWorldTransform() const;
        bool IsActive() const;
        void SetIsActive(bool b);
        void SetColliderScale(const btVector3& scale);
        void ApplyForce(vec3 direction, float force);
        void SetGravity(vec3 acceleration);
        entt::entity GetOwner();
        SharedPtr<btRigidBody> GetRigidbody() { return m_Rigidbody; }
        bool IsStatic() { return m_isStatic; };
    private:
        void Detach();

        entt::entity m_Owner;
        bool m_isStatic = false;
        bool m_isActive = true;
        bool m_Override = false;

        btTransform m_LastTransform{};
        double m_AccTime = 0.0;
        btVector3 m_OverriddenScale{ 1, 1, 1 };

        std::unique_ptr<btCompoundShape>     m_ShiftedCompoundShape;
        std::unique_ptr<btDefaultMotionState> m_MotionState;
        SharedPtr<btRigidBody>               m_Rigidbody;
        WeakPtr<PhysicsWorld>                m_World;
        SharedPtr<CollisionShape>            m_CollisionShape;
        std::unordered_set<Rigidbody*>       m_CurrentCollidedObjects;
    };
}
