#pragma once

#include "at/core/Core.h"
#include <at/ecs/Component.h>
#include <at/renderer/Material.h>
#include <at/renderer/Model.h>
namespace at
{
	struct AT_API MeshRenderer : public Component
	{
		MeshRenderer(std::shared_ptr<Model> m, std::shared_ptr<Material> mat) : Model(m), Material(mat){}
		std::shared_ptr<Model> Model;
		std::shared_ptr<Material> Material;

		const std::vector<Mesh>& GetMeshes()
		{
			return Model->m_Meshes;
		}
	};
}