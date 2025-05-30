#pragma once
#include "atpch.h"
#include <at/core/Core.h>
#include <entt/entity/registry.hpp>
#include "Scene.h"
#include "at/utils/Helpers.h"
#include "at/ecs/CoreComponents/ToBeDestroyedTag.h"
namespace at
{
	class Transform;
	class ToBeDestroyedTag;
	class AT_API Entity
	{
	public:
		Entity(entt::entity handle, std::shared_ptr<Scene> scene);
		Entity() = default;


		SharedPtr<Scene> GetScene()
		{
			return m_scene;
		}

		template<typename T, typename = std::enable_if_t < std::is_base_of_v<Entity, T>>>
		static T* Instantiate()
		{
			return Scene::ActiveScene->CreateEntity();
		}


		template<typename T, typename = std::enable_if_t < std::is_base_of_v<Entity, T>>>
		static T* Instantiate(Transform t)
		{
			return Scene::ActiveScene->CreateEntity(t);
		}

	
		void DestroyEntity();

		template<typename T>
		T& GetComponent()
		{
			return m_scene->m_registry.get<T>(m_handle);
		};

		template<typename T>
		bool HasComponent()
		{
			return m_scene->m_registry.any_of<T>(m_handle);
		};

		template<typename T, typename... Args>
		decltype(auto) AddComponent(Args&&... args)
		{
			assert(!HasComponent<T>() && "Entity already has component!");
			
			return m_scene->m_registry.emplace<T>(
				m_handle,
				std::forward<Args>(args)...
			);
		}

		template<typename T, typename... Args>
		T& AddOrReplaceComponent(Args&&... args)
		{
			return m_scene->m_registry.emplace_or_replace<T>(m_handle, std::forward<Args>(args)...);
		};

		
		// returns true if component is removed
		template<typename T>
		bool RemoveComponent()
		{
			return m_scene->m_registry.remove<T>(m_handle) != 0; 
		};

		bool operator==(const Entity& other)
		{
			return other.m_handle == m_handle;
		}

		bool operator!= (const Entity& other)
		{
			return other.m_handle != m_handle;
		}

		operator bool()
		{
			return m_scene != nullptr;
		}

		operator entt::entity()
		{
			return m_handle;
		}

	private:
		entt::entity m_handle;
		std::shared_ptr<Scene> m_scene;
	};
	
}

