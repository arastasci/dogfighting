#include "Application.h"
#include <GLFW/glfw3.h>
#include "Logger.h"
#include "at/renderer/Renderer.h"
namespace at {
	 Application* Application::m_instance;

	Application* Application::Instance()
	{
		return m_instance;
	}
	Application::Application()
	{
		
	}

	Application::~Application()
	{

	}

	void Application::Init()
	{
		if (m_instance == nullptr)
		{
			m_instance = this;
		}
		else
		{
			AT_CORE_CRITICAL("Application has multiple instances!");
			assert(m_instance == nullptr);
		}

		m_currentWindow = new Window();
		m_currentWindow->Init();

	}

	const float vertices[]
		=
	{
		-0.5f, -0.5f, 0.f,
		0.5f, -0.5f, 0.f,
		0.f, 0.5f, 0.f,
	};

	 uint32_t indices[] =
	{
		0, 1, 2
	};
	
	void Application::Run()
	{
		m_FrameTime = glfwGetTime();
		
		VertexBuffer b(sizeof(vertices));
		b.SetData(vertices, sizeof(vertices));

		BufferLayout l({ BufferElement(ShaderDataType::Float3, "aPos")});
		b.SetLayout(l);

		VertexArray a;

		auto e = std::make_shared<IndexBuffer>(indices, 3);

		auto bPtr = std::make_shared<VertexBuffer>(std::move(b));
		a.AddVertexBuffer(bPtr);

		auto aPtr = std::make_shared<VertexArray>(std::move(a));
		aPtr->SetIndexBuffer(e);

		Shader shader("res/shaders/vertex.glsl", "res/shaders/frag.glsl");

		while (!m_currentWindow->ShouldClose())
		{
			m_DeltaTime = glfwGetTime() - m_FrameTime;
			//Update();
			Renderer::DrawElements(aPtr, shader);
			m_currentWindow->SwapBuffers();
			glfwPollEvents();
		}


		glfwTerminate();
	}

	Window* Application::GetWindow()
	{
		return m_currentWindow;
	}

	void Application::Update()
	{

		//Scene::ActiveScene()->Update(m_DeltaTime);
	}

}
