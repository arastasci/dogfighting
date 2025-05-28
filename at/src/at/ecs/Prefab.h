#pragma once
#include "atpch.h"
#include "at/core/Core.h"
#include "at/core/Logger.h"
#include "at/core/EngineSubsystem.h"
#include "entt/meta/meta.hpp"
namespace at
{

	struct PrefabTag
	{
		PrefabTag(const std::string& name) : Name(name) {}
		std::string Name;
	};

	class Entity;

	class Prefab
	{
	public:
		std::string Name;

		virtual void InitEntity(Entity e) = 0;
	protected:
		Prefab()
		{
			//entt::meta<T>()
			//	.type(entt::hashed_string(entt::type_name<T>().value()));
		}

	};


	class AT_API PrefabLibrary : public EngineSubsystem<PrefabLibrary>
	{
	public:
		Prefab* GetPrefab(const std::string& name)
		{
			if (m_Prefabs.find(name) == m_Prefabs.end())
			{
				AT_CORE_CRITICAL("Prefab {} not in library!", name);
				return nullptr;
			}

			return m_Prefabs[name];
		}
		Prefab* GetPrefab(uint32_t guid)
		{
			if (m_GUIDtoNameMap.find(guid) == m_GUIDtoNameMap.end())
			{
				AT_CORE_CRITICAL("Prefab {} not in library!", guid);
				return nullptr;
			}

			return m_Prefabs[m_GUIDtoNameMap[guid]];
		}

		uint32_t GetGUID(const std::string& name)
		{
			if (m_NametoGUIDMap.find(name) == m_NametoGUIDMap.end())
			{
				AT_CORE_CRITICAL("Prefab {} not in library!", name);
				return 0;
			}

			return m_NametoGUIDMap[name];
		}

		bool AddPrefab(const std::string& name, Prefab* p)
		{
			if (m_Prefabs.find(name) != m_Prefabs.end())
			{
				AT_CORE_CRITICAL("Prefab {} already in library!", name);
				return false;
			}
			auto guid = guidCounter++;
			m_NametoGUIDMap[name] = guid;
			m_GUIDtoNameMap[guid] = name;
			m_Prefabs[name] = p;
			p->Name = name;
			return true;
		}

	private:
		uint32_t guidCounter = 1;
		std::unordered_map <std::string, uint32_t> m_NametoGUIDMap;
		std::unordered_map <uint32_t, std::string> m_GUIDtoNameMap;
		std::unordered_map<std::string, Prefab*> m_Prefabs;

	};

}