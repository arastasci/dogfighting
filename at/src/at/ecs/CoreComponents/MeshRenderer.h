#pragma once

#include "at/core/Core.h"
#include <at/ecs/Component.h>
#include <at/renderer/Material.h>
#include <at/renderer/Mesh.h>
namespace at
{
	struct AT_API MeshRenderer : public Component
	{
		Mesh Mesh;
		Material Material;
	};
}