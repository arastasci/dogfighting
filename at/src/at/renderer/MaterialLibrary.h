#pragma once
#include "atpch.h"
#include "at/core/EngineSubsystem.h"
#include "Material.h"
#include "ShaderLibrary.h"

namespace at
{
	class MaterialLibrary : public EngineSubsystem<MaterialLibrary>
	{
	public:
		void Init() override
		{
			m_Paths["defaultMaterial"] = std::make_pair< std::string, std::string>("res/shaders/lit_v.glsl", "res/shaders/lit_f.glsl");
		}

		std::shared_ptr<Material> CreateOrGetMaterial(const std::string& name)
		{
			if (m_Materials.find(name) != m_Materials.end())
			{
				AT_CORE_INFO("Material {} already created.", name);
				return m_Materials[name];
			}
			auto& pair = m_Paths[name];
			m_Materials[name] = std::make_shared<Material>(ShaderLibrary::Get().CreateOrGetShader(pair.first, pair.second, name));
			return m_Materials[name];
		}

		std::shared_ptr<Material> GetMaterial(const std::string& name)
		{
			return m_Materials[name];
		}

	private:
		std::unordered_map<std::string, std::pair<std::string, std::string>> m_Paths;
		std::unordered_map<std::string, std::shared_ptr<Material>> m_Materials;
	};
}