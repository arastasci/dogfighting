#include "Scene.h"
#include <at.h>
#include "Entity.h"
#include "entt/entt.hpp"
#include "at/renderer/RenderWorld.h"
#include "at/ecs/CoreComponents/CreatedTag.h"
#include "at/ecs/CoreComponents/ToBeDestroyedTag.h"

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
		e.AddComponent<CreatedTag>();
		e.AddComponent<Transform>(t);

		return e;
	}

	void Scene::Start()
	{
		m_SystemScheduler->Start();
	}

	void Scene::Update(double deltaTime)
	{
		m_SystemScheduler->Update(deltaTime);
	}

	void Scene::PostUpdate(double deltaTime)
	{
		m_SystemScheduler->PostUpdate(deltaTime);
	}

	void Scene::FixedUpdate(double deltaTime)
	{
		m_PhysicsWorld->Update(deltaTime);
		m_SystemScheduler->FixedUpdate(deltaTime);
	}
	void Scene::OnDestroy()
	{
		m_SystemScheduler->OnDestroy();
	}
	void Scene::EndFrame()
	{
		auto view = GetAllEntitiesWith<ToBeDestroyedTag>();
		for (auto [e, _] : view.each())
		{
			m_registry.destroy(e);
		}
	}
}
