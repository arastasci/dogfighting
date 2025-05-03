#pragma once
#include "atpch.h"
#include "Texture2D.h"
#include "glm/vec3.hpp"
#include "Shader.h"
namespace at
{
	struct Material
	{
		Material(std::shared_ptr<Shader> s) : Shader(s)
		{

		}
		//std::shared_ptr<Texture2D> MainTexture;
		//glm::vec3 BaseColor;
		//std::unordered_map<std::string, glm::vec3> UniformValues;
		std::shared_ptr<Shader> Shader;
	};
}