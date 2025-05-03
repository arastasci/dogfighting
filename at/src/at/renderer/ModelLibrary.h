#pragma once
#include "atpch.h"
#include "at/core/EngineSubsystem.h"
#include "Model.h"

namespace at
{
	class ModelLibrary : public EngineSubsystem<ModelLibrary>
	{
	public:
		std::shared_ptr<Model> LoadModel(const std::string& path, const std::string& name )
		{

			if (m_Models.find(name) != m_Models.end())
			{
				AT_CORE_INFO("Model {} already loaded.", name);
				return m_Models[name];
			}
			auto modelPtr = Model::CreateModel(path);

			m_Models[name] = modelPtr;
			return modelPtr;
		}

		std::shared_ptr<Model> GetModel(std::string& name)
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
	};
}