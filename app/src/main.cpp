 #include <at.h>

#include <random>
#include "FollowCamera.h"
#include "PlaneController.h"
#include "CollisionCallback.h"
#include "GoofySystem.h"
#include "RocketSystem.h"
#include "AppMessages.h"
using namespace at;


class PlanePrefab : Prefab<PlanePrefab>
{
public:
    void InitEntity(Entity e) override
    {
        e.AddComponent<MeshRenderer>("plane", "defaultMaterial");
        e.AddComponent<Rigidbody>();
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

        auto target = m_Scene->CreateNetworkedPrefab<PlanePrefab>(Transform(spawnPos));
        target.AddComponent<Spawner>();
    }
    int amount = 0;
    int maxAmount = 20;
};

struct FreeCamera : public BehaviourComponent
{
    float yaw = 0.0f;   
    float pitch = 0.0f; 
    float moveSpeed = 5.0f;  
    float mouseSensitivity = 0.1f;   
    glm::vec2 lastMousePos = { 0.0f, 0.0f };
    bool firstFrame = true;  
};

class FreeCameraSystem : public at::System
{
public:
    void Update(float dt) override
    {
        using namespace glm;
        auto view = GetView<at::Transform, FreeCamera>();

        auto [mx, my] = Input::GetMousePos();

        for (auto [e, tr, cam] : view.each())
        {
            if (Input::GetMouseButtonPress(GLFW_MOUSE_BUTTON_LEFT))
            {
                Input::SetCursorMode(Input::CursorMode::Locked);
                cam.firstFrame = false;
            }

            if (Input::GetKeyPress(Key::Escape) &&
                Input::GetCursorMode() == Input::CursorMode::Hidden)
            {
                Input::SetCursorMode(Input::CursorMode::Normal);
                cam.firstFrame = true;            
                return;                            
            }

            const vec2  delta = vec2(mx, my) - cam.lastMousePos;
            cam.lastMousePos = { mx, my };

            cam.yaw -= delta.x * cam.mouseSensitivity;
            cam.pitch -= delta.y * cam.mouseSensitivity;

            cam.pitch = clamp(cam.pitch, -89.0f, 89.0f);

            const float yawRad = radians(cam.yaw);
            const float pitchRad = radians(cam.pitch);

            vec3 forward{
                cos(pitchRad) * sin(yawRad),
                sin(pitchRad),
                cos(pitchRad) * cos(yawRad)
            };
            forward = normalize(forward);

            const vec3 worldUp = vec3{ 0, 1, 0 };
            vec3 right = normalize(cross(forward, worldUp));
            vec3 up = cross(right, forward);

            float v = cam.moveSpeed * dt;
            if (Input::GetKeyPress(Key::Up)) tr.position += forward * v;
            if (Input::GetKeyPress(Key::Down)) tr.position -= forward * v;
            if (Input::GetKeyPress(Key::Left)) tr.position -= right * v;
            if (Input::GetKeyPress(Key::Right)) tr.position += right * v;
            if (Input::GetKeyPress(Key::P))
            {
                m_WillPause = true;
            }
            else if (m_WillPause && Input::GetKeyRelease(Key::P))
            {
                m_WillPause = false;
                
                auto pW = m_Scene->GetPhysicsWorld();
                pW->SetIsSimulated(!pW->GetIsSimulated());
            }

            tr.rotation = glm::quatLookAt(forward, up);
        }
    }
private:
    bool m_WillPause = false;
};


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
        Networking::Get().SetHandleServerAppMessageCallback(Messages::HandleAppMessages_Server);
        Networking::Get().SetHandleClientAppMessageCallback(Messages::HandleAppMessages_Client);
        m_activeScene->AddSystem<FreeCameraSystem>();
        m_activeScene->AddSystem<GoofySystem>();
        m_activeScene->AddSystem<RocketSystem>();
        m_activeScene->AddSystem<FollowCameraSystem>();
        m_activeScene->AddSystem<CollisionSystem>();
        m_activeScene->AddSystem<PlaneControllerSystem>();
        m_activeScene->AddSystem<SpawnerSystem>();
		auto e = m_activeScene->CreateEntity(Transform(vec3(0, 0, 0)));


        e.AddComponent<MeshRenderer>("plane","defaultMaterial");

		e.AddComponent<DirectionalLightComponent>(DirectionalLight{ vec3(0, -1, 1), vec3(0.05f), vec3(0.4f), vec3(0.5f) });
		auto e2 = m_activeScene->CreateEntity(Transform(vec3(1, 0, -1)));
		e2.AddComponent<PointLightComponent>(
			1, 0.09f, 0.032f,
			vec3(0.05f), vec3(0.8f), vec3(1.0f)
			);

        auto terrainEntity = m_activeScene->CreateEntity(Transform(vec3(0, -60, 0), quat(), vec3(10, 5, 10)));
        terrainEntity.AddComponent<MeshRenderer>("terrain", "defaultMaterial");
        terrainEntity.AddComponent<Rigidbody>(true);
		e.AddComponent<Rigidbody>();

        auto rocket = m_activeScene->CreateEntity(Transform(vec3(0, 30, 0), quat(), vec3(0.1f)));
        rocket.AddComponent<MeshRenderer>("rocket", "defaultMaterial");
        rocket.AddComponent<Rigidbody>(false, true);

        auto camera = m_activeScene->CreateEntity(Transform(&e.GetComponent<Transform>(), vec3(-2.5f, 2.5f, -6.0f), quat(), vec3(1.0f)));

		camera.AddComponent<CameraComponent>(camera.GetComponent<Transform>().position, Vector3::forward, Vector3::up, 45.0f, 1280.f / 720.f);
        camera.AddComponent<FreeCamera>();
        e.AddComponent<PlaneController>();
        e.AddComponent<CollisionCallback>();
        

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