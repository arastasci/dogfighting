#pragma once
#include <variant>
#include <type_traits>
#include "atpch.h"
#include "Texture2D.h"
#include "at/utils/Math.h"
#include "Shader.h"

namespace at
{
	using UniformVariant = std::variant<
		bool,
		int,                       // bool / int / sampler2D, …
		float,                     // float
		glm::vec2, glm::vec3, glm::vec4,
		glm::mat3, glm::mat4
	>;

	template<class... Ts>
	struct overloaded : Ts... {          // inherits every lambda
		using Ts::operator()...;         // and exposes all their operator()
	};
	template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;  // CTAD guide

	struct Material
	{
		Material(std::shared_ptr<Shader> s) : MatShader(s)
		{

		}
		//std::shared_ptr<Texture2D> MainTexture;
		vec4 BaseColor{ 1, 1, 1, 1 };
		std::unordered_map<std::string, UniformVariant> UniformValues{};
		std::shared_ptr<Shader> MatShader;

		void UploadUniforms();


		void SetUniform(const std::string& name, const UniformVariant& v);


	};
}