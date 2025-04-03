#pragma once
#include "at/core/Core.h"
#include <at/ecs/Component.h>

namespace at
{
	struct AT_API BehaviourComponent : public Component
	{

		template<typename T, typename = std::enable_if_t < std::is_base_of_v<Entity, T>>>
		static T* Instantiate()
		{
			return Entity::Instantiate<T>();
		}


		template<typename T, typename = std::enable_if_t < std::is_base_of_v<Entity, T>>>
		static T* Instantiate(Transform t)
		{
			return Entity::Instantiate<T>(t);
		}


		template<typename T, typename = std::enable_if_t<std::is_base_of_v<Component, T>>>
		T& GetComponent()
		{
			return m_Entity.GetComponent<T>();
		};

		template<typename T, typename = std::enable_if_t<std::is_base_of_v<Component, T>>>
		bool HasComponent()
		{
			return m_Entity.HasComponent<T>();
		};

		template<typename T, typename = std::enable_if_t<std::is_base_of_v<Component, T>>, typename... Args>
		T& AddComponent(Args&&... args)
		{
			return m_Entity.AddComponent<T>();
		};

		template<typename T, typename = std::enable_if_t<std::is_base_of_v<Component, T>>, typename... Args>
		T& AddOrReplaceComponent(Args&&... args)
		{
			return m_Entity.AddOrReplaceComponent<T>();
		};

		// remove returns the num of elements removed so
		// returns true if component is removed
		template<typename T, typename = std::enable_if_t<std::is_base_of_v<Component, T>>>
		bool RemoveComponent()
		{
			return m_Entity.RemoveComponent<T>();
		};
	};
}