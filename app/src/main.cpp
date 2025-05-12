 #include <at.h>

#include <random>
#include "FollowCamera.h"
#include "PlaneController.h"


using namespace at;


class GoofyBehaviour : public BehaviourComponent
{
};

class GoofySystem : public System
{
public:
    GoofySystem() = default;

    void Start() override
    {
        auto view = GetStartedView<GoofyBehaviour, Rigidbody>();
        for (auto& [e, _, g, rb] : view.each())
        {
            vec3 forceDirection(static_cast <float> (rand()) / static_cast <float> (RAND_MAX), static_cast <float> (rand()) / static_cast <float> (RAND_MAX), static_cast <float> (rand()) / static_cast <float> (RAND_MAX));
            forceDirection = glm::normalize(forceDirection);
            float force = 10000000 * static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
            rb.ApplyForce(forceDirection, force);
        }
    }

    void Update(float dt) override
    {
        if (Input::GetMouseButtonPress(GLFW_MOUSE_BUTTON_RIGHT))
        {
            CreatePlane();
        }
    }
private:
    void CreatePlane()
    {
        auto backpack = m_Scene->CreateEntity(Transform(vec3(3, 50, 0)));
        backpack.AddComponent<GoofyBehaviour>();
        backpack.AddComponent<MeshRenderer>(ModelLibrary::Get().CreateOrGetModel("res/models/plane/plane.fbx", "plane"), MaterialLibrary::Get().CreateOrGetMaterial("res/shaders/lit_v.glsl", "res/shaders/lit_f.glsl", "defaultMaterial"));
        backpack.AddComponent<Rigidbody>();
    }

};


struct FreeCamera : public BehaviourComponent
{
    float yaw = 0.0f;   // degrees, 0 = look +Z
    float pitch = 0.0f;   // degrees, 0 = level, + up
    float moveSpeed = 5.0f;   // units / second
    float mouseSensitivity = 0.1f;   // deg per pixel
    glm::vec2 lastMousePos = { 0.0f, 0.0f };
    bool firstFrame = true;    // initialise lastMousePos
};

class FreeCameraSystem : public at::System
{
public:
    void Update(float dt) override
    {
        using namespace glm;
        auto view = GetView<at::Transform, FreeCamera>();

        // read current mouse position once per frame
        auto [mx, my] = Input::GetMousePos();

        for (auto [e, _, tr, cam] : view.each())
        {
            /* first run ‑ enable pointer lock */
            if (Input::GetMouseButtonPress(GLFW_MOUSE_BUTTON_LEFT))
            {
                Input::SetCursorMode(Input::CursorMode::Locked);
                cam.firstFrame = false;
            }

            /* ESC releases the mouse so people can use the UI again */
            if (Input::GetKeyPress(Key::Escape) &&
                Input::GetCursorMode() == Input::CursorMode::Hidden)
            {
                Input::SetCursorMode(Input::CursorMode::Normal);
                cam.firstFrame = true;             // triggers re‑lock on next click
                return;                            // skip camera update this frame
            }

            const vec2  delta = vec2(mx, my) - cam.lastMousePos;
            cam.lastMousePos = { mx, my };

            cam.yaw += delta.x * cam.mouseSensitivity;
            cam.pitch += delta.y * cam.mouseSensitivity;

            // clamp pitch to avoid flipping
            cam.pitch = clamp(cam.pitch, -89.0f, 89.0f);

            /* ── 2. Build camera basis vectors ─────────────────────────── */
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

            /* ── 3. Keyboard movement (camera‑relative) ────────────────── */
            float v = cam.moveSpeed * dt;
            if (Input::GetKeyPress(Key::Up)) tr.position -= forward * v;
            if (Input::GetKeyPress(Key::Down)) tr.position += forward * v;
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

            /* ── 4. Write rotation quaternion to the Transform ─────────── */
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
        m_activeScene->AddSystem<FreeCameraSystem>();
        m_activeScene->AddSystem<GoofySystem>();
        m_activeScene->AddPostSystem<FollowCameraSystem>();
        m_activeScene->AddSystem<PlaneControllerSystem>();
		auto e = m_activeScene->CreateEntity(Transform(vec3(0, 0, 0)));

        e.AddComponent<MeshRenderer>(ModelLibrary::Get().CreateOrGetModel("res/models/plane/plane.fbx", "plane"), MaterialLibrary::Get().CreateOrGetMaterial("res/shaders/lit_v.glsl", "res/shaders/lit_f.glsl", "defaultMaterial"));

		e.AddComponent<DirectionalLightComponent>(DirectionalLight{ vec3(0, -1, -1), vec3(0.05f), vec3(0.4f), vec3(0.5f) });
		auto e2 = m_activeScene->CreateEntity(Transform(vec3(1, 0, -1)));
		e2.AddComponent<PointLightComponent>(
			1, 0.09f, 0.032f,
			vec3(0.05f), vec3(0.8f), vec3(1.0f)
			);

        auto terrainEntity = m_activeScene->CreateEntity(Transform(vec3(0, -60, 0), quat(), vec3(0.5f)));
        terrainEntity.AddComponent<MeshRenderer>(ModelLibrary::Get().CreateOrGetModel("res/models/lowpoly_terrain/lowpoly_terrain.obj", "terrain"), MaterialLibrary::Get().CreateOrGetMaterial("res/shaders/lit_v.glsl", "res/shaders/lit_f.glsl", "defaultMaterial"));
        terrainEntity.AddComponent<Rigidbody>(true);
		auto rb = e.AddComponent<Rigidbody>();

        
        //{
        //    auto backpack = m_activeScene->CreateEntity(Transform(vec3(3, 50, 0)));
        //    backpack.AddComponent<MeshRenderer>(ModelLibrary::Get().CreateOrGetModel("res/models/plane/plane.fbx", "plane"), MaterialLibrary::Get().CreateOrGetMaterial("res/shaders/lit_v.glsl", "res/shaders/lit_f.glsl", "defaultMaterial"));
        //    backpack.AddComponent<Rigidbody>();
        //}
       
        auto camera = m_activeScene->CreateEntity(Transform(/*&e.GetComponent<Transform>(),*/ vec3(2.5f, 2.5f, -6.0f), quat(vec3(0,90,0)), vec3(1.0f)));

		camera.AddComponent<CameraComponent>(camera.GetComponent<Transform>().position, Vector3::forward, Vector3::up, 45.0f, 1280.f / 720.f);
        camera.AddComponent<FreeCamera>();
        e.AddComponent<PlaneController>();

        //camera.AddComponent<FollowCamera>(e.GetComponent<Transform>(), vec3(0, 3.0f, -6.0f));

	}
};




int  main()
{ 
	at::Logger::Init();
	AT_INFO("Well, we lent some code from {0}...", "Cherno");

	Sandbox* s = new Sandbox();
	s->Init();
	s->Run();
}