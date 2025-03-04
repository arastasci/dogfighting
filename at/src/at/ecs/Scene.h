#pragma once
#include <entt/entity/registry.hpp>
#include <at/ecs/CoreComponents/CoreComponents.h>

namespace at
{
	class Entity;

	class Scene
	{
	public:
		static Scene* ActiveScene();

		Scene();
		~Scene();
		void Init();
		Entity CreateEntity();
		Entity CreateEntity(const Transform& t);

	private:
		static Scene* m_activeScene;

		entt::registry m_registry;

		friend class Entity;
	};

}
