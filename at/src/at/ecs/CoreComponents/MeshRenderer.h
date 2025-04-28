#pragma once

#include "at/core/Core.h"
#include <at/ecs/Component.h>
#include <at/renderer/Material.h>
#include <at/renderer/Model.h>
namespace at
{
	struct AT_API MeshRenderer : public Component
	{
		MeshRenderer(Model&& m, Material&& mat) : Model(m), Material(mat){}
		Model Model;
		Material Material;

		std::vector<Mesh> GetMeshes()
		{
			return Model.m_Meshes;
		}
	};
}