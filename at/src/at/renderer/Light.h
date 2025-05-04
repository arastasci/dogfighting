#pragma once

#include "atpch.h"
#include "at/utils/Math.h"
namespace at
{
	struct PointLight
	{
		float constant;
		float linear;
		float quadratic;

		vec3 ambient;
		vec3 diffuse;
		vec3 specular;
	};

	struct PointLightRenderInfo
	{
		vec3 position;
		PointLight pointLight;
	};

	struct DirectionalLight 
	{
		vec3 direction{};

		vec3 ambient{};
		vec3 diffuse{};
		vec3 specular{};
	};
}