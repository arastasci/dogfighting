#pragma once
#include "atpch.h"
#include "at/core/Core.h"
#include "at/core/EngineSubsystem.h"
#include "btBulletDynamicsCommon.h"
#include "at/utils/Helpers.h"
#include "at/utils/Constants.h"

namespace at
{
	

	class Rigidbody;

	class AT_API PhysicsWorld 
		: public EngineSubsystem<PhysicsWorld>
	{
	public:
		explicit PhysicsWorld()
		{
			m_CollisionConfiguration = new
				btDefaultCollisionConfiguration();
			m_Dispatcher = new btCollisionDispatcher(m_CollisionConfiguration);
			m_OverlappingPairCache = new btDbvtBroadphase();
			m_Solver = new btSequentialImpulseConstraintSolver();
			m_World = std::make_unique<btDiscreteDynamicsWorld>(m_Dispatcher,
					m_OverlappingPairCache, m_Solver, m_CollisionConfiguration);
			instance = this;
		}

		void AddRigidbody(btRigidBody* rb)
		{
			m_World->addRigidBody(rb);
		}

		void UpdateAABB(btRigidBody* rb)
		{
			m_World->updateSingleAabb(rb);
		}

		void Update(float dt);

		virtual void Init() override;

		virtual void Shutdown() override;

		void GetCollidingObjects(std::vector<Rigidbody*>& colliders, Rigidbody* rb);

		void SetIsSimulated(bool isSimulated) 
		{
			m_isSimulated = isSimulated;
		}

		void UpdateLastTransforms();

		bool GetIsSimulated() { return m_isSimulated; }
	private:

		void UpdateCollisions();
		
		bool m_isSimulated = true;
		struct PairRigidHash {
			size_t operator()(Pair<Rigidbody*, Rigidbody*> const& p) const noexcept
			{
				auto h1 = std::hash<Rigidbody*>{}(p.first);
				auto h2 = std::hash<Rigidbody*>{}(p.second);
				return h1 ^ (h2 + 0x9e3779b97f4a7c15ULL + (h1 << 6) + (h1 >> 2));
			}
		};

		struct PairRigidEq {
			bool operator()(Pair<Rigidbody*, Rigidbody*> const& a,
				Pair<Rigidbody*, Rigidbody*> const& b) const noexcept
			{
				return a.first == b.first && a.second == b.second;
			}
		};

		std::unordered_map<
			Pair<Rigidbody*, Rigidbody*>,
			btPersistentManifold*,
			PairRigidHash,
			PairRigidEq> m_CurrentCollisionMap;

		std::unordered_map<
			Pair<Rigidbody*, Rigidbody*>,
			btPersistentManifold*,
			PairRigidHash,
			PairRigidEq> m_PreviousCollisionMap;

		std::unique_ptr<btDiscreteDynamicsWorld> m_World;

		btDefaultCollisionConfiguration* m_CollisionConfiguration;
		btCollisionDispatcher* m_Dispatcher;
			/// btDbvtBroadphase is a good general purpose broadphase . You can also try out btAxis3Sweep .
		btBroadphaseInterface * m_OverlappingPairCache;
		/// the default constraint solver . For parallel processing you can use a different solver
			//(see Extras / BulletMultiThreaded)
		btSequentialImpulseConstraintSolver * m_Solver;
		
	};

}