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
			newConvexHull->addPoint(Math::toBt(p.Position));
		}
		
		m_Shapes.push_back(newConvexHull);
		btTransform bT(Math::toBt(mesh->GetModelMatrix()));
		m_CompoundShape->addChildShape(bT, newConvexHull);
		m_Masses.push_back(1.0f);
		
	}

	void CollisionShape::AddStaticMesh(const Mesh* mesh)
	{
		const auto& verts = mesh->GetVertices();
		const auto& indices = mesh->GetIndices();

		auto* triMesh = new btTriangleMesh(/*use32bitIndices=*/true,
			/*use4componentVertices=*/false);

		// ❸ Feed every triangle to Bullet
		for (std::size_t i = 0; i + 2 < indices.size(); i += 3)
		{
			const vec3& p0 = verts[indices[i]].Position;
			const vec3& p1 = verts[indices[i + 1]].Position;
			const vec3& p2 = verts[indices[i + 2]].Position;

			triMesh->addTriangle(Math::toBt(p0),Math::toBt(p1),Math::toBt(p2));
		}

		const bool useQuantizedAabbCompression = true;
		auto* meshShape = new btBvhTriangleMeshShape(triMesh,
			useQuantizedAabbCompression);


		btTransform modelXf(Math::toBt(mesh->GetModelMatrix()));
		m_CompoundShape->addChildShape(modelXf, meshShape);
		m_Shapes.push_back(meshShape);
		m_Masses.push_back(0.0f);
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
