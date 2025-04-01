 #include <at.h>
using namespace at;
class Sandbox : public at::Application
{
public:
	Sandbox()
	{

	}
	
	~Sandbox()
	{

	}
	virtual void AppInit() override
	{
		auto shader = std::make_shared<Shader>("res/shaders/vertex.glsl", "res/shaders/frag.glsl");

		auto e = m_activeScene->CreateEntity();

		e.AddComponent<MeshRenderer>(Mesh(), Material(shader));

		auto camera = m_activeScene->CreateEntity(Transform(vec3(0, 0, -3)));

		camera.AddComponent<CameraComponent>(camera.GetComponent<Transform>().position, Vector3::forward, Vector3::up, 45.0f, 1280.f / 720.f);

	}
};


int  main()
{ 
	at::Logger::Init();
	AT_INFO("Well, we lent some code from {0}...", "Cherno");
	Sandbox* a = new Sandbox();
	a->Init();

	a->Run();
	delete a;
}