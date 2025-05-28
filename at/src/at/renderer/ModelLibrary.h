#pragma once
#include "atpch.h"
#include "at/core/Core.h"
#include "at/core/EngineSubsystem.h"
#include "Model.h"

namespace at
{
	class AT_API ModelLibrary : public EngineSubsystem<ModelLibrary>
	{
	public:
		void Init() override
		{
			m_Paths["plane"] = "res/models/plane/plane.fbx";
			m_Paths["rocket"] = "res/models/rocket/rocket.fbx";
			m_Paths["terrain"] = "res/models/meshy/terrain.obj";

		}

		std::shared_ptr<Model> CreateOrGetModel(const std::string& name )
		{

			if (m_Models.find(name) != m_Models.end())
			{
				AT_CORE_INFO("Model {} already loaded.", name);
				return m_Models[name];
			}
			auto modelPtr = Model::CreateModel(m_Paths[name], name);

			m_Models[name] = modelPtr;
			return modelPtr;
		}

		std::shared_ptr<Model> GetModel(const std::string& name)
		{
			if (m_Models.find(name) == m_Models.end())
			{
				AT_CORE_CRITICAL("Model {} not in model library!", name);
				return nullptr;
			}

			return m_Models[name];
		}

	private:
		std::unordered_map<std::string, std::shared_ptr<Model>> m_Models;
		
		std::unordered_map<std::string, std::string> m_Paths;
	};
}