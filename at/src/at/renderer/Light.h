#pragma once

#include "atpch.h"
#include "at/utils/Math.h"
namespace at
{
	struct PointLight
	{
		vec3 position;
		float constant;
		float linear;
		float quadratic;

		vec3 ambient;
		vec3 diffuse;
		vec3 specular;
	};

	// global
	struct DirectionalLight 
	{
		uint32_t id;

		vec3 direction;

		vec3 ambient;
		vec3 diffuse;
		vec3 specular;
	};
}