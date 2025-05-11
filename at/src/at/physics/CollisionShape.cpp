#pragma once 

#include "CollisionShape.h"
#include "at/renderer/Mesh.h"

namespace at
{
	CollisionShape::CollisionShape()
	{
		m_CompoundShape = new btCompoundShape();
	}

	void CollisionShape::AddMesh(const Mesh* mesh)
	{
		auto& points = mesh->GetVertices();
		auto newConvexHull = new btConvexHullShape();
		for (auto& p : points)
		{
			newConvexHull->addPoint(toBt(p.Position));
		}
		
		m_Shapes.push_back(newConvexHull);
		btTransform bT(toBt(mesh->GetModelMatrix()));
		m_CompoundShape->addChildShape(bT, newConvexHull);
		m_Masses.push_back(1.0f);
		
	}

	void CollisionShape::DebugRender()
	{
		btTransform t;
		btVector3 min, max;
	}

	btCompoundShape* CollisionShape::GetShape()
	{
		return m_CompoundShape;
	}

	void CollisionShape::CalculatePrincipalAxisTransform(btTransform& principal, btVector3& inertia)
	{
		m_CompoundShape->calculatePrincipalAxisTransform(m_Masses.data(), principal, inertia);
	}

}
