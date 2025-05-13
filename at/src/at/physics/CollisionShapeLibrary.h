#pragma once
#include "atpch.h"

#include "at/core/EngineSubsystem.h"
#include "CollisionShape.h"
#include "at/core/Logger.h"
#include "at/utils/Helpers.h"
#include "at/renderer/Model.h"
#include "at/renderer/ModelLibrary.h"
namespace at {
	class AT_API CollisionShapeLibrary : public EngineSubsystem<CollisionShapeLibrary>
	{
	public:
		std::shared_ptr<CollisionShape> CreateOrGetCollisionShape(const std::string& name, bool isStatic)
		{

			if (m_ShapeMap.find(name) != m_ShapeMap.end())
			{
				AT_CORE_INFO("Model {} already loaded.", name);
				return m_ShapeMap[name];
			}
			auto modelPtr = std::make_shared<CollisionShape>();

			if (!isStatic)
			{
				auto meshes = ModelLibrary::Get().GetModel(name)->GetMeshes();

				for (auto& mesh : meshes)
				{
					modelPtr->AddMesh(mesh);
				}

			}
			else
			{
				auto meshes = ModelLibrary::Get().GetModel(name)->GetMeshes();

				for (auto& mesh : meshes)
				{
					modelPtr->AddStaticMesh(mesh);
				}
			}
			m_ShapeMap[name] = modelPtr;
			return modelPtr;
		}

		std::shared_ptr<CollisionShape> GetCollisionShape(std::string& name)
		{
			if (m_ShapeMap.find(name) == m_ShapeMap.end())
			{
				AT_CORE_CRITICAL("Model {} not in model library!", name);
				return nullptr;
			}

			return m_ShapeMap[name];
		}

	private:
		std::unordered_map<std::string, SharedPtr<CollisionShape>> m_ShapeMap{};
	};
}