#include "Scene.h"
#include <at.h>
#include "Entity.h"
#include "entt/entt.hpp"
#include "at/renderer/RenderWorld.h"

namespace at
{
	std::shared_ptr<Scene> Scene::m_activeScene;

	std::shared_ptr<Scene> Scene::ActiveScene()
	{
		return m_activeScene;
	}
	Scene::Scene()
	{

	}

	Scene::~Scene()
	{
	}

	void Scene::Init()
	{
		if (m_activeScene == nullptr)
		{
			m_activeScene = shared_from_this();
			m_SystemScheduler = std::make_unique<SystemScheduler>();
			m_SystemScheduler->m_Scene = shared_from_this();
			m_PhysicsWorld = std::make_shared<PhysicsWorld>();
			m_PhysicsWorld->Init();
		}
		else
		{
			AT_CORE_WARN("A passive scene has been initialized");
		}
	}


	Entity Scene::CreateEntity()
	{
		Transform t;
		return CreateEntity(t);
	}

	Entity Scene::CreateEntity(const Transform& t)
	{
		auto handle = m_registry.create();

		Entity e = { handle, shared_from_this()};

		e.AddComponent<Transform>(t);

		return e;
	}

	void Scene::Update(double deltaTime)
	{
		m_PhysicsWorld->Update(deltaTime);
		m_SystemScheduler->Update(deltaTime);
	}
}
