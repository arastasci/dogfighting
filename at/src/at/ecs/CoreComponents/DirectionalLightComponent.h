#pragma once
#include "atpch.h"
#include "at/renderer/Light.h"


namespace at
{
	struct DirectionalLightComponent 
	{
		DirectionalLightComponent(DirectionalLight l) : light(l)
		{

		}

		DirectionalLight light;
	};
}