#pragma once 

#include "atpch.h"
#include "at/core/Logger.h"
#include "at/renderer/Light.h"
#include "at/utils/Constants.h"
#include "at/core/EngineSubsystem.h"
#include "at/renderer/Shader.h"

namespace at
{
	class RenderWorld : public EngineSubsystem<RenderWorld>
	{
	public:
		void SetPointLight(const PointLight& light, vec3 position)
		{
			if (pointLightIndex == Constants::PointLightCount)
			{
				AT_CORE_WARN("Maximum pointLight count of {} exceeded!", pointLightIndex);
				return;
			}
			pointLights[pointLightIndex++] = {position, light};
		}

		void SetDirectionalLight(DirectionalLight& light)
		{
			directionalLight = light;
		}

		void RegisterLights(std::shared_ptr<Shader> shader)
		{
			if(directionalLight.has_value())
			{
				std::string name = "dirLight.";

				shader->setVec3(name + "direction", directionalLight.value().direction);
				shader->setVec3(name + "ambient", directionalLight.value().ambient);
				shader->setVec3(name + "diffuse", directionalLight.value().diffuse);
				shader->setVec3(name + "specular", directionalLight.value().specular);
			}
			for (int i = 0; i < pointLightIndex; i++)
			{
				auto& light = pointLights[i].pointLight;
				auto& pos = pointLights[i].position;
				std::string name = "pointLights[" + std::to_string(i) + "].";
				shader->setVec3(name + "position", pos);

				shader->setVec3(name + "ambient", light.ambient);
				shader->setVec3(name + "specular", light.specular);
				shader->setVec3(name + "diffuse", light.diffuse);

				shader->setFloat(name + "constant", light.constant);
				shader->setFloat(name + "linear", light.linear);
				shader->setFloat(name + "quadratic", light.quadratic);
			}
			shader->setInt("pointLightCount", pointLightIndex);
		}

		void Flush()
		{
			pointLightIndex = 0;
		}

	private:
		std::optional<DirectionalLight> directionalLight;
		PointLightRenderInfo pointLights[Constants::PointLightCount];
		short pointLightIndex{ 0 };
	};
}
