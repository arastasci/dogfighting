#pragma once

#include "Rigidbody.h"
#include "PhysicsWorld.h"
#include "at/ecs/CoreComponents/MeshRenderer.h"
namespace at
{
	Rigidbody::Rigidbody() 		
	{
		m_CollisionShape = std::make_shared<CollisionShape>();
		m_MotionState = new btDefaultMotionState();
	}

	void Rigidbody::AddBodyToWorld(const SharedPtr<PhysicsWorld>& world)
	{
		m_World = std::weak_ptr(world);

		if (m_Entity.HasComponent<MeshRenderer>())
		{
			auto& mr = m_Entity.GetComponent<MeshRenderer>();
			auto& meshes = mr.GetMeshes();
			for (auto& m : meshes)
			{
				m_CollisionShape->AddMesh(m);
			}
		}

		m_Rigidbody = std::make_shared<btRigidBody>(1, m_MotionState , m_CollisionShape->GetShape());
		
		world->AddRigidbody(m_Rigidbody.get());

		btTransform transform = m_Rigidbody->getCenterOfMassTransform();
		transform.setOrigin(toBt(m_Entity.GetComponent<Transform>().position));               //set position
		transform.setBasis(toBt(glm::mat3(m_Entity.GetComponent<Transform>().rotation)));   //set orientation
		m_Rigidbody->setCenterOfMassTransform(transform);
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
		btTransform btT;
		m_Rigidbody->getMotionState()->getWorldTransform(btT);
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

	void Rigidbody::DrawShape()
	{
		
	}

	void Rigidbody::ApplyForce(vec3 direction, float force)
	{
		//m_Rigidbody->applyCentralForce(toBt(force * direction));
		m_Rigidbody->applyCentralImpulse(toBt(force * direction));	
	}

	void Rigidbody::SetGravity(vec3 acceleration)
	{
		m_Rigidbody->setGravity(toBt(acceleration));
	}

}