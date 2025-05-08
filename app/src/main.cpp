 #include <at.h>
using namespace at;


class MoveBehaviour : public BehaviourComponent
{

};


class MoveSystem : public System
{
	virtual void Update(float dt) override
	{
		auto view = GetView<Transform, MoveBehaviour>();
		for (auto [e, t, m] : view.each())
		{
			if (Input::GetKeyPress(Key::W))
			{
				t.position += vec3(0, 0, 1) * dt;
			}
			if (Input::GetKeyPress(Key::S))
			{
				t.position += vec3(0, 0, -1) * dt;
			}
			if (Input::GetKeyPress(Key::A))
			{
				t.position += vec3(1, 0, 0) * dt;
			}
			if (Input::GetKeyPress(Key::D))
			{
				t.position += vec3(-1, 0, 0) * dt;
			}
		}
		
	}
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
		m_activeScene->AddSystem<MoveSystem>();

		auto e = m_activeScene->CreateEntity();

		e.AddComponent<MeshRenderer>(ModelLibrary::Get().CreateOrGetModel("res/models/backpack/backpack.obj", "backpack"), MaterialLibrary::Get().CreateOrGetMaterial("res/shaders/lit_v.glsl", "res/shaders/lit_f.glsl", "defaultMaterial"));
		e.AddComponent<DirectionalLightComponent>(DirectionalLight{ vec3(0, -1, -1), vec3(0.05f), vec3(0.4f), vec3(0.5f) });
		auto e2 = m_activeScene->CreateEntity(Transform(vec3(1, 0, -1)));
		e2.AddComponent<PointLightComponent>(
			1, 0.09f, 0.032f,
			vec3(0.05f), vec3(0.8f), vec3(1.0f)
			);
		e.AddComponent<Rigidbody>();
		auto camera = m_activeScene->CreateEntity(Transform(vec3(0, 0, -3)));

		camera.AddComponent<CameraComponent>(camera.GetComponent<Transform>().position, Vector3::forward, Vector3::up, 45.0f, 1280.f / 720.f);
		camera.AddComponent<MoveBehaviour>();
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