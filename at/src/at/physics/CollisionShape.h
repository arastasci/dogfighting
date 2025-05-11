#pragma once
#include "atpch.h"

#include "btBulletCollisionCommon.h"
namespace at
{
	class Mesh;
	class CollisionShape
	{
	public:
		CollisionShape();
		void AddMesh(const Mesh* mesh);
		
		void DebugRender();

		btCompoundShape* GetShape();
		void CalculatePrincipalAxisTransform(btTransform& principal, btVector3& inertia);

	private:
		btCompoundShape* m_CompoundShape;
		std::vector<btConvexHullShape*> m_Shapes{};
		std::vector<float> m_Masses;
	};
}