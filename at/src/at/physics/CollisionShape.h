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
		void AddMesh(const Mesh& mesh);
		
		void DebugRender();

		btConvexHullShape* GetShape();
	private:
		btConvexHullShape* m_Shape;
	};
}