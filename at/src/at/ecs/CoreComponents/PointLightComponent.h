#pragma once
#include "atpch.h"

#include "at/renderer/Light.h"
namespace at
{
	struct PointLightComponent 
	{
		PointLightComponent(float constant, float linear, float quadratic, vec3 ambient, vec3 diffuse, vec3 specular)
			: PointLight {constant, linear, quadratic, ambient, diffuse, specular}
		{

		}

		PointLight PointLight;
	};
}