#pragma once

#include <at/ecs/Component.h>
#include <at/renderer/Material.h>
#include "at/renderer/VertexArray.h"
namespace at
{
	struct MeshRenderer : public Component
	{
		VertexArray VertexArray;

	};
}