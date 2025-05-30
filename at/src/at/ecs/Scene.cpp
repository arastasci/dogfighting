#include <at.h>
#include "Scene.h"
#include "Entity.h"
#include "entt/entt.hpp"
#include "at/renderer/RenderWorld.h"
#include "at/ecs/CoreComponents/CreatedTag.h"
#include "at/ecs/CoreComponents/ToBeDestroyedTag.h"
#include "at/ecs/CoreComponents/ActiveTag.h"
#include "at/networking/NetworkTag.h"
#include "at/renderer/MeshRendererSystem.h"
#include "at/physics/PhysicsSystem.h"
#include <at/networking/DirtyComponent.h>
#include <at/networking/NetworkSyncSystem.h>
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
			m_PhysicsSystem = std::make_unique<PhysicsSystem>();
			m_PhysicsSystem->SetScene(shared_from_this());
			m_RendererSystem = std::make_unique<MeshRendererSystem>();
			m_RendererSystem->SetScene(shared_from_this());
			m_SyncSystem = std::make_unique<NetworkSyncSystem>();
			m_SyncSystem->SetScene(shared_from_this());
		}
		else
		{
			AT_CORE_WARN("A passive scene has been initialized");
		}
	}
	Entity Scene::CreateNetworkedPrefab(const std::string& prefabName)
	{
		Transform t;
		return CreateNetworkedPrefab(prefabName, t);
	}
	Entity Scene::CreateNetworkedPrefab(const std::string& prefabName, const Transform& t)
	{
		auto e = CreateEntity(t);

		PrefabLibrary::Get().GetPrefab(prefabName)->InitEntity(e);
		e.AddComponent<PrefabTag>(prefabName);
		e.AddComponent<NetworkTag>();

		return e;
	}

	Entity Scene::CreatePrefab(const std::string& prefabName)
	{
		Transform t;
		return CreatePrefab(prefabName, t);
	}

	Entity Scene::CreatePrefab(const std::string& prefabName, const Transform& t)
	{
		auto e = CreateEntity(t);
		PrefabLibrary::Get().GetPrefab(prefabName)->InitEntity(e);
		return e;
	}

	Entity Scene::CreateNetworkedEntity()
	{
		Transform t;
		return CreateNetworkedEntity(t);
	}

	Entity Scene::CreateNetworkedEntity(const Transform& t)
	{
		auto e = CreateEntity(t);
		e.AddComponent<NetworkTag>();
		e.AddComponent<DirtyComponent>();
		// TODO: broadcast networked entity creation
		return e;
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

	Entity Scene::GetEntity(entt::entity e)
	{
		return { e, shared_from_this() };
	}

	bool Scene::IsValidEntity(entt::entity e)
	{
		return m_registry.valid(e);
	}

	bool Scene::DestroyEntity(entt::entity e)
	{
		if (!m_registry.valid(e))
			return false;
		m_registry.destroy(e);
		return true;
	}

	void Scene::SetComponentCreatedCallback(OnComponentCreatedCallback callback)
	{
		m_OnComponentCreatedCallback = callback;
	}

	void Scene::PreUpdate()
	{
		auto view = GetAllEntitiesWith<CreatedTag>();

		for (auto [e_] : view.each())
		{
			Entity e = { e_, shared_from_this()};
			e.RemoveComponent<CreatedTag>();
			e.AddComponent<ActiveTag>();
		}
	}

	void Scene::Start()
	{
		m_PhysicsSystem->Start();
		m_SystemScheduler->Start();
	}

	void Scene::Update(double deltaTime)
	{
		m_SystemScheduler->Update(deltaTime);
	}

	void Scene::LateUpdate(double deltaTime)
	{
		m_SystemScheduler->LateUpdate(deltaTime);
	}

	double Scene::FixedUpdate(double dt)
	{
		m_AccTimestep += dt;
		m_TotalTime = glfwGetTime();

		bool willSimulate = false;
		if (m_AccTimestep >= Constants::FIXED_TIMESTEP)
		{
			m_PhysicsWorld->UpdateLastTransforms();
			willSimulate = true;
		}
		
		while (m_AccTimestep >= Constants::FIXED_TIMESTEP)
		{
			m_PhysicsWorld->Update(dt);
			m_SystemScheduler->FixedUpdate(dt);

			m_AccTimestep -= Constants::FIXED_TIMESTEP;
		}
		if (willSimulate)
		{
			m_SyncSystem->FixedUpdate();
		}
		return m_AccTimestep;
	}
	void Scene::InterpolatePhysicsTransforms(double t)
	{
		m_PhysicsSystem->InterpolateTransforms(t);
	}

	void Scene::Render()
	{
		m_RendererSystem->UpdateCameraProperties();
		m_RendererSystem->RegisterPointLights();
		m_RendererSystem->RenderEntities();
		m_RendererSystem->DrawCubemap();
	}

	void Scene::OnDestroy()
	{
		m_SystemScheduler->OnDestroy();
	}
	void Scene::EndFrame()
	{
		auto view = GetAllEntitiesWith<ToBeDestroyedTag>();
		m_registry.destroy(view.begin(), view.end());
	}
}
