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
			if (Input::GetKeyPress(AT_KEY_W))
			{
				t.position += vec3(0, 0, 1) * dt;
			}
			if (Input::GetKeyPress(AT_KEY_S))
			{
				t.position += vec3(0, 0, -1) * dt;
			}
			if (Input::GetKeyPress(AT_KEY_A))
			{
				t.position += vec3(1, 0, 0) * dt;
			}
			if (Input::GetKeyPress(AT_KEY_D))
			{
				t.position += vec3(-1, 0, 0) * dt;
			}
		}
		
	}
};

class SandboxGame : public at::GameLayer
{
public:
	void GameInit() override
	{
		m_activeScene->m_SystemScheduler->Register(std::make_shared<MoveSystem>());
		auto shader = std::make_shared<Shader>("res/shaders/vertex.glsl", "res/shaders/frag.glsl");

		auto e = m_activeScene->CreateEntity();

		e.AddComponent<MeshRenderer>(Mesh(), Material(shader));

		auto camera = m_activeScene->CreateEntity(Transform(vec3(0, 0, -3)));

		camera.AddComponent<CameraComponent>(camera.GetComponent<Transform>().position, Vector3::forward, Vector3::up, 45.0f, 1280.f / 720.f);
		camera.AddComponent<MoveBehaviour>();
	} 
};


int main()
{ 
	at::Logger::Init();
	AT_INFO("Well, we lent some code from {0}...", "Cherno");

	Application* s = new Application();
	s->Init();

	SandboxGame* game = new SandboxGame();

	s->PushLayer(game);
	s->Run();
}