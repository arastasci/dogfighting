#pragma once
#include "atpch.h"
#include "at/renderer/Light.h"
#include "at/ecs/Component.h"

namespace at
{
	struct DirectionalLightComponent : public Component
	{
		DirectionalLightComponent(DirectionalLight l) : light(l)
		{

		}

		DirectionalLight light;
	};
}