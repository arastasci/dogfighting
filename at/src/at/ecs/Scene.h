#pragma once
#include "at/core/Core.h"
#include <entt/entity/registry.hpp>
#include "SystemScheduler.h"

namespace at
{
	class Entity;
	class Transform;
	class CameraComponent;
	class AT_API Scene : public std::enable_shared_from_this<Scene>
	{
	public:
		static std::shared_ptr<Scene> ActiveScene();

		Scene();
		~Scene();
		void Init();
		Entity CreateEntity();
		Entity CreateEntity(const Transform& t);
		void Update(double deltaTime);

		template<typename... Components>
		auto GetAllEntitiesWith()
		{
			return m_registry.view<Components...>();
		}

		template<typename... Components>
		auto GetAllComponents()
		{
			return m_registry.group<Components...>();
		}

		template<typename T,
			typename = std::enable_if_t<std::is_base_of_v<ISystem, T>>>
		std::shared_ptr<T> AddSystem()
		{
			auto system = std::make_shared<T>();
			m_SystemScheduler->Register(system);
			return system;
		}


	private:

		// TOOO: change ltr
		std::unique_ptr<SystemScheduler> m_SystemScheduler;
		static std::shared_ptr<Scene> m_activeScene;
		static CameraComponent* m_MainCamera;

		entt::registry m_registry;

		friend class Entity;

	};

}
