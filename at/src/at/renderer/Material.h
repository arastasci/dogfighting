#pragma once
#include "atpch.h"
#include "Texture2D.h"
#include "glm/vec3.hpp"
namespace at
{
	struct Material
	{
		std::shared_ptr<Texture2D> MainTexture;
		glm::vec3 BaseColor;
		std::unordered_map<std::string, glm::vec3> UniformValues;
	};
}