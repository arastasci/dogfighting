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
		std::shared_ptr<Material> CreateOrGetMaterial(const std::string& vShader, const std::string& fShader, const std::string& name)
		{
			if (m_Materials.find(name) != m_Materials.end())
			{
				AT_CORE_INFO("Material {} already created.", name);
				return m_Materials[name];
			}

			m_Materials[name] = std::make_shared<Material>(ShaderLibrary::Get().CreateOrGetShader(vShader, fShader, name));
			return m_Materials[name];
		}

		std::shared_ptr<Material> GetMaterial(const std::string& name)
		{
			return m_Materials[name];
		}

	private:
		std::unordered_map<std::string, std::shared_ptr<Material>> m_Materials;
	};
}