#pragma once
#include "at/core/Core.h"
#include <entt/entity/registry.hpp>
#include "SystemScheduler.h"

#include "at/utils/Helpers.h"
#include "at/physics/PhysicsWorld.h"
#include "ComponentTypes.h"
#include "Prefab.h"
namespace at
{
	class Entity;
	class Transform;
	class CameraComponent;
	class MeshRendererSystem;
	class PhysicsSystem;
	class NetworkSyncSystem;
	class AT_API Scene : public std::enable_shared_from_this<Scene>
	{
	public:
		using OnComponentCreatedCallback = std::function<void(entt::entity, size_t)>;
		static std::shared_ptr<Scene> ActiveScene();

		Scene();
		~Scene();
		void Init();

		Entity CreateNetworkedPrefab(const std::string& prefabName);
		Entity CreateNetworkedPrefab(const std::string& prefabName, const Transform& t);
		Entity CreatePrefab(const std::string& prefabName);

		Entity CreatePrefab(const std::string& prefabName, const Transform& t);
		
		Entity CreateNetworkedEntity();
		Entity CreateNetworkedEntity(const Transform& t);
		Entity CreateEntity();
		Entity CreateEntity(const Transform& t);
		Entity GetEntity(entt::entity e);
		bool IsValidEntity(entt::entity e);
		bool DestroyEntity(entt::entity e);
		void SetComponentCreatedCallback(OnComponentCreatedCallback);
		template<typename T>
		void OnComponentCreated(entt::entity e)
		{
		}
		void PreUpdate();
		void Start();
		void Update(double deltaTime);
		void LateUpdate(double deltaTime);
		double FixedUpdate(double dT);
		void InterpolatePhysicsTransforms(double t);
		void Render();
		void OnDestroy();
		void EndFrame();

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


		const SharedPtr<PhysicsWorld> GetPhysicsWorld()
		{
			return m_PhysicsWorld;
		}

		entt::registry& GetRegistry()
		{
			return m_registry;
		}


	private:
		OnComponentCreatedCallback m_OnComponentCreatedCallback;
		float m_TotalTime = 0;
		float m_AccTimestep = 0;
		// TOOO: change ltr
		std::unique_ptr<SystemScheduler> m_SystemScheduler;
		static std::shared_ptr<Scene> m_activeScene;
		static CameraComponent* m_MainCamera;
		SharedPtr<PhysicsWorld> m_PhysicsWorld;
		std::unique_ptr<PhysicsSystem> m_PhysicsSystem;
		std::unique_ptr<MeshRendererSystem> m_RendererSystem;
		std::unique_ptr<NetworkSyncSystem> m_SyncSystem;
		entt::registry m_registry;

		friend class Entity;

	};

}
