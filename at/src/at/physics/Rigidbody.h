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
namespace at
{
	class RigidbodyComponent;
	class PhysicsWorld;
	class AT_API Rigidbody : public Component
	{
	public:
		Rigidbody();
		void AddBodyToWorld(const SharedPtr<PhysicsWorld>& world);
		void GetCollidingObjects(std::vector<Rigidbody*>& colliders);
		
		void ClearCollidedObjects();
		void ReportCollidedObject(Rigidbody* rb);
		Transform getWorldTransform() const;
		bool IsActive();
		void SetIsActive(bool b);

	private:
		bool m_isActive = false;
		SharedPtr<btRigidBody> m_Rigidbody;
		WeakPtr<PhysicsWorld> m_World;
		SharedPtr<btCompoundShape> m_CompoundShape;
		btMotionState* m_MotionState{};

		std::unordered_set<Rigidbody*> m_CurrentCollidedObjects{};
	};
}