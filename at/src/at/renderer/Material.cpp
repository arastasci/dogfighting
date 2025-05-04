#include "Material.h"

namespace at
{
	void Material::UploadUniforms()
	{
		MatShader->use();
		MatShader->setVec4("baseColor", BaseColor);
		for (auto& [name, var] : UniformValues)
		{
			SetUniform(name, var);
		}
	}

	void Material::SetUniform(const std::string& name, const UniformVariant& v)
	{
		
		std::visit(overloaded{
			[&](bool value) { MatShader->setBool(name, value); },
			[&](int  value) { MatShader->setInt(name, value); },
			[&](float value) { MatShader->setFloat(name, value); },
			[&](const glm::vec2& value) { MatShader->setVec2(name, value); },
			[&](const glm::vec3& value) { MatShader->setVec3(name, value); },
			[&](const glm::vec4& value) { MatShader->setVec4(name, value); },
			[&](const glm::mat3& value) { MatShader->setMat3(name, value); },
			[&](const glm::mat4& value) { MatShader->setMat4(name, value); }
			// if you later add new types to UniformVariant,
			// just add another lambda here.
			}, v);
		
	}
}