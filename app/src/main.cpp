 #include <at.h>

#include <random>
#include "FollowCamera.h"
#include "PlaneController.h"
#include "CollisionCallback.h"
#include "GoofySystem.h"
#include "RocketSystem.h"
#include "AppMessages.h"
#include "FreeCamera.h"
using namespace at;


class PlanePrefab : public Prefab
{
public:
    void InitEntity(Entity e) override
    {
        e.AddComponent<MeshRenderer>("plane", "defaultMaterial");
        e.AddComponent<Rigidbody>();
        e.AddComponent<PlayerPlaneController>();
        e.AddComponent<PlaneFlightBehaviour>();
    }
};


struct Spawner
{

};

class SpawnerSystem : public System
{
public:
    void Start()
    {
        auto view = GetStartedView<Spawner, Rigidbody>();
        for (auto [e, rb] : view.each())
        {
            rb.SetGravity(vec3(0));
        }
    }
    void Update(float dt) override
    {
        if (amount < maxAmount)
        {
            Spawn();
            amount++;
        }
    }

    void FixedUpdate()
    {
        auto view = GetView<Spawner, MeshRenderer, Rigidbody>();
        for (auto [e, mr, rb] : view.each())
        {
            Entity en = { e, m_Scene };
            std::vector<Rigidbody*> colliders;
            rb.GetCollidingObjects(colliders);
            for (auto c : colliders)
            {
                Entity cEn = { c->GetOwner(), m_Scene };
                if ( cEn.HasComponent<RocketBehaviour>())
                {
                    en.DestroyEntity();
                    break;
                }
            }
        }
    }
private: 
    void Spawn()
    {
        vec3 spawnDir(static_cast <float> (rand()) / static_cast <float> (RAND_MAX), static_cast <float> (rand()) / static_cast <float> (RAND_MAX), static_cast <float> (rand()) / static_cast <float> (RAND_MAX));
        spawnDir -= vec3(0.5f);
        vec3 spawnPos(100.f * spawnDir.x, 50 + 30.f * spawnDir.y, 100.f * spawnDir.z);

        auto target = m_Scene->CreateNetworkedPrefab("plane", Transform(spawnPos));
        target.AddComponent<Spawner>();
    }
    int amount = 0;
    int maxAmount = 20;
};



void OnClientConnected(std::shared_ptr<Scene> scene, HSteamNetConnection conn)
{
    auto& nc = Networking::Get();
    auto* msg = new Messages::ConnectedMessage(conn);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    nc.SendToClient(conn, msg, sizeof(*msg), true);
}

class Sandbox : public at::Application
{
public:
	Sandbox() : Application()
	{

	}
	
	~Sandbox()
	{

	}
	virtual void AppInit() override
	{
        PrefabLibrary::Get().AddPrefab("plane", new PlanePrefab());
        PrefabLibrary::Get().AddPrefab("rocket", new RocketPrefab());

        Networking::Get().SetHandleServerAppMessageCallback(Messages::HandleAppMessages_Server);
        Networking::Get().SetHandleClientAppMessageCallback(Messages::HandleAppMessages_Client);
        Networking::Get().SetClientConnectedCallback(OnClientConnected);
        m_activeScene->AddSystem<FreeCameraSystem>();
        m_activeScene->AddSystem<GoofySystem>();
        if (Networking::Get().IsHost())
        {
            m_activeScene->AddSystem<RocketSystem>();
            m_activeScene->AddSystem<PlaneFlightSystem>();  
            m_activeScene->AddSystem<SpawnerSystem>();
        }
        m_activeScene->AddSystem<PlayerPlaneControllerSystem>();
        m_activeScene->AddSystem<FollowCameraSystem>();
        m_activeScene->AddSystem<CollisionSystem>();

        auto& nc = Networking::Get();
        if(nc.IsHost())
            while (!nc.HostNotAlone())
            {
                nc.Update();
            }
        else
        {
            while (!nc.IsConnectedToHost())
                nc.Update();
        }
		auto e = m_activeScene->CreateEntity(Transform(vec3(0, 0, 0)));


		e.AddComponent<DirectionalLightComponent>(DirectionalLight{ vec3(0, -1, 1), vec3(0.05f), vec3(0.4f), vec3(0.5f) });
		auto e2 = m_activeScene->CreateEntity(Transform(vec3(1, 0, -1)));
		e2.AddComponent<PointLightComponent>(
			1, 0.09f, 0.032f,
			vec3(0.05f), vec3(0.8f), vec3(1.0f)
			);

        auto terrainEntity = m_activeScene->CreateEntity(Transform(vec3(0, -60, 0), quat(), vec3(10, 5, 10)));
        terrainEntity.AddComponent<MeshRenderer>("terrain", "defaultMaterial");
        terrainEntity.AddComponent<Rigidbody>(true);


        /*auto camera = m_activeScene->CreateEntity(Transform(&e.GetComponent<Transform>(), vec3(-2.5f, 2.5f, -6.0f), quat(), vec3(1.0f)));

		camera.AddComponent<CameraComponent>(camera.GetComponent<Transform>().position, Vector3::forward, Vector3::up, 45.0f, 1280.f / 720.f);
        camera.AddComponent<FreeCamera>();*/
        
        

	}
};




int  main()
{ 
	at::Logger::Init();
	AT_INFO("Initializing the application.");

	Sandbox* s = new Sandbox();
	s->Init();
	s->Run();
}