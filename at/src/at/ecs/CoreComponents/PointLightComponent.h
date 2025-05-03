#pragma once
#include "atpch.h"
#include "at/ecs/Component.h"

namespace at
{
	struct PointLightComponent : public Component
	{
		PointLight PointLight;
	};
}