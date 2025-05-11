 #include <at.h>
using namespace at;


class MoveBehaviour : public BehaviourComponent
{

};

class FlightControls : public BehaviourComponent
{
    const float ForwardThrust = 100.f;
};

class FlightSystem : public System
{
public:
    void FixedUpdate() override
    {
        auto view = GetView<Rigidbody, FlightControls>();

        for (auto [e, rb, f] : view.each())
        {
            rb.ApplyForce(vec3(0, 9.8f, 0), Constants::FIXED_TIMESTEP);
            // PlaneAerodynamics(rb.GetRigidbody().get());
        }
    }

private:
    // called each tick from a pre‑tick callback or btActionInterface
    void PlaneAerodynamics(btRigidBody* body)
    {
        /* --------- 1. basic data we need each tick ------------------------ */
        btVector3 v = body->getLinearVelocity();
        btScalar   speed = v.length();
        if (speed < 1e-2f) return;                       // too slow → no aero

        btVector3 velDir = v / speed;                    // unit velocity (world)

        const btMatrix3x3& basis = body->getWorldTransform().getBasis();
        btVector3 fwdWorld = basis.getColumn(2).normalized();   // +X is “nose”

        /* --------- 2. how badly are we mis‑aligned? ----------------------- */
        btScalar cosθ = fwdWorld.dot(velDir);            //  1 → perfect alignment
        btScalar sinθ = (fwdWorld.cross(velDir)).length(); //  0 → aligned, 1 → 90°

        /* If the nose is already within ~10° of the velocity vector
           we don’t need any more pitch‑down force. */
        if (cosθ > 0.985f) return;                       // ≈ 10 deg threshold

        /* --------- 3. drag force (opposite to velocity) ------------------- */
        const btScalar ρ = 1.225f;                     // kg m‑3 (sea level)
        const btScalar CdA = 1.5f;                       // crude reference area
        btVector3 drag = -0.5f * ρ * CdA * speed * speed * velDir;

        /* --------- 4. pick a Cp that rides with the nose ------------------ */
        const btScalar cpDist = 3.0f;                    // metres ahead of COM
        btVector3 cpWorld = body->getCenterOfMassPosition() + fwdWorld * cpDist;

        /* --------- 5. fade the force as we get closer to alignment -------- */
        drag *= sinθ;                                    // zero when aligned

        /* --------- 6. apply it – Bullet converts (r×F) to the right torque */
        body->applyForce(drag, cpWorld - body->getCenterOfMassPosition());
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

        for (auto [e, tr, cam] : view.each())
        {
            /* first run ‑ enable pointer lock */
            if (Input::GetMouseButtonPress(GLFW_MOUSE_BUTTON_LEFT))
            {
                Input::SetCursorMode(Input::CursorMode::Locked);
                cam.firstFrame = false;
            }

            /* ESC releases the mouse so people can use the UI again */
            if (Input::GetKeyPress(Key::Escape) &&
                Input::GetCursorMode() == Input::CursorMode::Locked)
            {
                Input::SetCursorMode(Input::CursorMode::Normal);
                cam.firstFrame = true;             // triggers re‑lock on next click
                return;                            // skip camera update this frame
            }

            const vec2  delta = vec2(mx, my) - cam.lastMousePos;
            cam.lastMousePos = { mx, my };

            cam.yaw -= delta.x * cam.mouseSensitivity;
            cam.pitch += delta.y * cam.mouseSensitivity;  // invert Y

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
            if (Input::GetKeyPress(Key::W)) tr.position -= forward * v;
            if (Input::GetKeyPress(Key::S)) tr.position += forward * v;
            if (Input::GetKeyPress(Key::A)) tr.position += right * v;
            if (Input::GetKeyPress(Key::D)) tr.position -= right * v;
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
        m_activeScene->AddSystem<FlightSystem>();

		auto e = m_activeScene->CreateEntity(Transform(vec3(0, 0, 0)));

        e.AddComponent<MeshRenderer>(ModelLibrary::Get().CreateOrGetModel("res/models/plane/plane.fbx", "plane"), MaterialLibrary::Get().CreateOrGetMaterial("res/shaders/lit_v.glsl", "res/shaders/lit_f.glsl", "defaultMaterial"));

		e.AddComponent<DirectionalLightComponent>(DirectionalLight{ vec3(0, -1, -1), vec3(0.05f), vec3(0.4f), vec3(0.5f) });
		auto e2 = m_activeScene->CreateEntity(Transform(vec3(1, 0, -1)));
		e2.AddComponent<PointLightComponent>(
			1, 0.09f, 0.032f,
			vec3(0.05f), vec3(0.8f), vec3(1.0f)
			);

        auto terrainEntity = m_activeScene->CreateEntity(Transform(vec3(0, -50, 0), quat(), vec3(0.1f)));
        terrainEntity.AddComponent<MeshRenderer>(ModelLibrary::Get().CreateOrGetModel("res/models/lowpoly_terrain/lowpoly_terrain.obj", "terrain"), MaterialLibrary::Get().CreateOrGetMaterial("res/shaders/lit_v.glsl", "res/shaders/lit_f.glsl", "defaultMaterial"));
        terrainEntity.AddComponent<Rigidbody>();
		auto rb = e.AddComponent<Rigidbody>();

        e.AddComponent<FlightControls>();
        auto backpack = m_activeScene->CreateEntity(Transform(vec3(3, 50, 0)));
        backpack.AddComponent<MeshRenderer>(ModelLibrary::Get().CreateOrGetModel("res/models/plane/plane.fbx", "plane"), MaterialLibrary::Get().CreateOrGetMaterial("res/shaders/lit_v.glsl", "res/shaders/lit_f.glsl", "defaultMaterial"));
        backpack.AddComponent<Rigidbody>();
		auto camera = m_activeScene->CreateEntity(Transform(vec3(0, 0, -3)));

		camera.AddComponent<CameraComponent>(camera.GetComponent<Transform>().position, Vector3::forward, Vector3::up, 45.0f, 1280.f / 720.f);
		camera.AddComponent<FreeCamera>();
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