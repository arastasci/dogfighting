#pragma once

#include "Rigidbody.h"
#include "PhysicsWorld.h"
namespace at
{
	Rigidbody::Rigidbody() 		
	{
		m_CompoundShape = std::make_shared<btCompoundShape>();
		m_MotionState = new btDefaultMotionState();
	}

	void Rigidbody::AddBodyToWorld(const SharedPtr<PhysicsWorld>& world)
	{
		m_World = std::weak_ptr(world);
		m_Rigidbody = std::make_shared<btRigidBody>(1, m_MotionState , m_CompoundShape.get());
		world->AddRigidbody(m_Rigidbody.get());
		m_Rigidbody->setUserPointer(this);
	}

	void Rigidbody::GetCollidingObjects(std::vector<Rigidbody*>& colliders)
	{
		m_World.lock()->GetCollidingObjects(colliders, this);
	}

	void Rigidbody::ClearCollidedObjects()
	{
		m_CurrentCollidedObjects.clear();
	}

	void Rigidbody::ReportCollidedObject(Rigidbody* body)
	{
		m_CurrentCollidedObjects.insert(body);
	}

	Transform Rigidbody::getWorldTransform() const
	{
		auto btT = m_Rigidbody->getWorldTransform();
		auto btQ = btT.getRotation();
		glm::quat q(btQ.w(), btQ.x(), btQ.y(), btQ.z());
		return Transform(toGlm(btT.getOrigin()), q, vec3(1.0f));
	}

	bool Rigidbody::IsActive()
	{
		return m_isActive;
	}

	void Rigidbody::SetIsActive(bool b)
	{
		m_isActive = b;
	}

}