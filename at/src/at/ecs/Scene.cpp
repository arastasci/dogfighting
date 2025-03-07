#include "Scene.h"
#include <at.h>
#include "Entity.h"
namespace at
{
	Scene* Scene::m_activeScene;

	Scene* Scene::ActiveScene()
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
			m_activeScene = this;
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

		Entity e = { handle, this };

		e.AddComponent<Transform>(t);

		return e;
	}
}
