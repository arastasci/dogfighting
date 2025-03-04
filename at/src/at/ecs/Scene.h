#pragma once
#include <entt/entity/registry.hpp>

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
		Entity* CreateEntity();

	private:
		static Scene* m_activeScene;

		entt::registry m_registry;

		friend class Entity;
	};

}
