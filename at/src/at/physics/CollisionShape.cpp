#pragma once 

#include "CollisionShape.h"
#include "at/renderer/Mesh.h"

namespace at
{
	CollisionShape::CollisionShape()
	{
		m_Shape = new btConvexHullShape();
	}

	void CollisionShape::AddMesh(const Mesh& mesh)
	{
		auto& points = mesh.GetVertices();

		for (auto& p : points)
		{
			m_Shape->addPoint(toBt(p.Position));
		}
	}

	void CollisionShape::DebugRender()
	{
		btTransform t;
		btVector3 min, max;
	}

	btConvexHullShape* CollisionShape::GetShape()
	{
		return m_Shape;
	}

}
