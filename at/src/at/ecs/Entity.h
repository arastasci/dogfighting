#pragma once
#include <at/core/Core.h>
#include <entt/entity/registry.hpp>
#include "Component.h"
#include "Scene.h"
#include <at/ecs/CoreComponents/CoreComponents.h>
namespace at
{
	class AT_API Entity
	{
	public:
		Entity(entt::entity handle, Scene* scene);

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


		template<typename T, typename = std::enable_if_t<std::is_base_of_v<Component, T>>>
		T& GetComponent()
		{
			return m_scene->m_registry.get<T>(m_handle);
		};

		template<typename T, typename = std::enable_if_t<std::is_base_of_v<Component, T>>>
		bool HasComponent()
		{
			return m_scene->m_registry.any_of<T>(m_handle);
		};

		template<typename T, typename = std::enable_if_t<std::is_base_of_v<Component, T>>, typename... Args>
		T& AddComponent(Args&&... args)
		{
			assert(!HasComponent<T>(), "Entity already has component!");
			return m_scene->m_registry.emplace<T>(m_handle, std::forward<Args>(args)...);
		};

		template<typename T, typename = std::enable_if_t<std::is_base_of_v<Component, T>>, typename... Args>
		T& AddOrReplaceComponent(Args&&... args)
		{
			return m_scene->m_registry.emplace_or_replace<T>(m_handle, std::forward<Args>(args)...);
		};

		// remove returns the num of elements removed so
		// returns true if component is removed
		template<typename T, typename = std::enable_if_t<std::is_base_of_v<Component, T>>>
		bool RemoveComponent()
		{
			return m_scene->m_registry.remove<T>(m_handle) != 0; 
		};

	private:
		entt::entity m_handle;
		std::shared_ptr<Scene> m_scene;
	};
}

