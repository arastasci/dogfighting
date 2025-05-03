#pragma once
#include "atpch.h"
#include "at/core/EngineSubsystem.h"
#include "Shader.h"
namespace at
{
	class ShaderLibrary : public EngineSubsystem<ShaderLibrary>
	{
	public:
		std::shared_ptr<Shader> CreateOrGetShader(const std::string& vShader, const std::string& fShader, const std::string& name)
		{
			if (m_Shaders.find(name) != m_Shaders.end())
			{
				AT_CORE_INFO("Shader {} already created.", name);
				return m_Shaders[name];
			}

			m_Shaders[name] = std::make_shared<Shader>(Shader(vShader, fShader));
			return m_Shaders[name];
		}

		std::shared_ptr<Shader> GetShader(const std::string& name)
		{
			return m_Shaders[name];
		}

	private:
		std::unordered_map<std::string, std::shared_ptr<Shader>> m_Shaders;
	};
}