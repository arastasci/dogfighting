#pragma once
#include "atpch.h"
#include "Mesh.h"
#include "at/utils/Math.h"
namespace at
{
	class ModelNode
	{
	public:
		ModelNode(ModelNode* p, const mat4& localMatrix) : m_Parent(p)
		{
			if (m_Parent != nullptr)
			{
				m_ModelMatrix = m_Parent->GetModelMatrix() * localMatrix;
			}
			else
			{
				m_ModelMatrix = localMatrix;
			}
		}
		
		const mat4& GetModelMatrix() const
		{
			return m_ModelMatrix;
		}

		void AddChild(ModelNode* child)
		{
			m_Children.push_back(child);
		}

		void AddMesh(Mesh* mesh)
		{
			m_Meshes.push_back(mesh);
		}

		const std::vector<Mesh*>& GetMeshes() const
		{
			return m_Meshes;
		}

	private:
		ModelNode* m_Parent;
		mat4 m_ModelMatrix{};
		std::vector<ModelNode*> m_Children{};
		std::vector<Mesh*> m_Meshes{};
	};
}