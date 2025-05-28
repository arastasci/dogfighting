#pragma once
#include "PhysicsWorld.h"
#include "Rigidbody.h"
#include "at/networking/Networking.h"
namespace at
{
	void PhysicsWorld::Update(float dt)
	{
		if (!m_isSimulated || !Networking::Get().IsHost())
			return;
		
		m_World->stepSimulation(Constants::FIXED_TIMESTEP, 0, Constants::FIXED_TIMESTEP);

		
		//m_World->stepSimulation(dt, maxSubstep, Constants::FIXED_TIMESTEP);
		UpdateCollisions();
	}

	void PhysicsWorld::Init()
	{
		m_World->setGravity(btVector3(0, -9.8f, 0));
	}

	void PhysicsWorld::Shutdown()
	{
		delete m_CollisionConfiguration;
		delete m_Dispatcher;
		delete m_OverlappingPairCache;
		delete m_Solver;
	}
	void PhysicsWorld::GetCollidingObjects(std::vector<Rigidbody*>& colliders, Rigidbody* rb)
	{
		for (auto it = m_CurrentCollisionMap.begin(); it != m_CurrentCollisionMap.end(); it++)
		{
			auto bodyPair = it->first;

			if (bodyPair.first == rb)
			{
				colliders.push_back(bodyPair.second);
			}
			else if (bodyPair.second == rb)
			{
				colliders.push_back(bodyPair.first);
			}
		}
	}
	void PhysicsWorld::UpdateLastTransforms()
	{
		auto rbs = m_World->getNonStaticRigidBodies();
		int num = rbs.size();

		for (int i = 0; i < num; i++)
		{
			auto* rbComponent = static_cast<Rigidbody*>(rbs[i]->getUserPointer());
			rbComponent->UpdateLastTransform();
		}
	}
	void at::PhysicsWorld::UpdateCollisions()
	{
		m_World->performDiscreteCollisionDetection();
		auto& rigidbodies = m_World->getNonStaticRigidBodies();
		for (int i = 0; i < rigidbodies.size(); i++)
		{
			auto* rb = static_cast<Rigidbody*>(rigidbodies.at(i)->getUserPointer());
			rb->ClearCollidedObjects();
		}
		m_CurrentCollisionMap.clear();

		int numManifolds = m_Dispatcher->getNumManifolds();

		for (int i = 0; i < numManifolds; i++)
		{
			auto manifold = m_Dispatcher->getManifoldByIndexInternal(i);

			int numContacts = manifold->getNumContacts();
			if (!numContacts) continue;

			auto* rb0 = static_cast<Rigidbody*>(manifold->getBody0()->getUserPointer());
			auto* rb1 = static_cast<Rigidbody*>(manifold->getBody1()->getUserPointer());
			if (rb0 && rb1)
			{
				std::pair<Rigidbody*, Rigidbody*> bodyPair;
				if (rb0 < rb1)
				{
					bodyPair = std::pair<Rigidbody*, Rigidbody*>(rb0, rb1);
				}
				else
				{
					bodyPair = std::pair<Rigidbody*, Rigidbody*>(rb1, rb0);
				}
				m_CurrentCollisionMap[bodyPair] = manifold;
			}
		}

		for (auto it = m_CurrentCollisionMap.begin(); it != m_CurrentCollisionMap.end(); it++)
		{
			auto bodyPair = it->first;
			auto manifold = it->second;
			bool newCollision = m_PreviousCollisionMap.find(bodyPair) == m_PreviousCollisionMap.end();

			if (newCollision)
			{
				bodyPair.first->ReportCollidedObject(bodyPair.second);
				bodyPair.second->ReportCollidedObject(bodyPair.first);
			}
		}

		m_PreviousCollisionMap = m_CurrentCollisionMap;
	}
}
