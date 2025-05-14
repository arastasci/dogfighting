#pragma once

#include "atpch.h"
#include "at/core/Core.h"
#include "at/utils/Helpers.h"
#include "at/utils/Math.h"
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <BulletCollision/CollisionShapes/btCompoundShape.h>
#include "at/ecs/CoreComponents/Transform.h"
#include "at/ecs/Component.h"

#include "CollisionShape.h"
namespace at
{
	class RigidbodyComponent;
	class PhysicsWorld;
	class AT_API Rigidbody : public Component
	{
	public:
		Rigidbody(bool isKinematic = false, bool override = false);
		~Rigidbody();
		void AddBodyToWorld(const SharedPtr<PhysicsWorld>& world);
		void GetCollidingObjects(std::vector<Rigidbody*>& colliders);
		void UpdateLastTransform();
		void ClearCollidedObjects();
		void ReportCollidedObject(Rigidbody* rb);
		Transform GetInterpolatedTransform(float t);
		Transform GetWorldTransform() const;
		Transform GetStaticWorldTransform() const;
		bool IsActive();
		void SetIsActive(bool b);
		void DrawShape();
		void SetColliderScale(const btVector3&);
		
		SharedPtr<btRigidBody> GetRigidbody() { return m_Rigidbody; }
		void ApplyForce(vec3 direction, float force);
		void SetGravity(vec3 acceleration);
	private:
		bool m_isStatic = false;
		bool m_isActive = false;
		btTransform m_LastTransform;
		bool m_Override;
		double m_AccTime;
		btVector3 m_OverriddenScale;
		SharedPtr<btRigidBody> m_Rigidbody;
		WeakPtr<PhysicsWorld> m_World;
		SharedPtr<CollisionShape> m_CollisionShape;
		btCompoundShape* m_ShiftedCompoundShape;
		btMotionState* m_MotionState{};

		std::unordered_set<Rigidbody*> m_CurrentCollidedObjects{};
	};
}