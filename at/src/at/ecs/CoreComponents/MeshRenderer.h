#pragma once

#include <at/ecs/Component.h>
#include <at/renderer/Material.h>
#include <at/renderer/Mesh.h>
namespace at
{
	struct MeshRenderer : public Component
	{
		Mesh Mesh;
		Material Material;
	};
}