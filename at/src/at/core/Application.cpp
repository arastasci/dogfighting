#include "Application.h"
#include <GLFW/glfw3.h>
#include "Logger.h"
#include "at/renderer/Renderer.h"
#include "at/ecs/CoreSystems/CameraSystem.h"
#include "at/ecs/CoreSystems/MeshRendererSystem.h"
#include "at/ecs/CoreSystems/PointLightSystem.h"
#include "at/physics/PhysicsSystem.h"

#include "Input.h"
namespace at {
	 Application* Application::m_instance;

	Application* Application::Instance()
	{
		return m_instance;
	}
	Application::Application()
	{
		m_activeScene = std::make_shared<Scene>();
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
		Renderer::Init();
		m_activeScene->Init();
		stbi_set_flip_vertically_on_load(true);


		m_activeScene->AddSystem<CameraSystem>();
		m_activeScene->AddSystem<PointLightSystem>();
		m_activeScene->AddSystem<PhysicsSystem>();
		m_activeScene->AddSystem<MeshRendererSystem>();


		AppInit(); // application initialization code
	}


	void Application::Run()
	{
		m_FrameTime = glfwGetTime();
		
		while (!m_currentWindow->ShouldClose())
		{

			auto timeNow = glfwGetTime();
			m_DeltaTime = timeNow - m_FrameTime;
			m_FrameTime = timeNow;

			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			m_activeScene->Update(m_DeltaTime);


			m_currentWindow->SwapBuffers();
			glfwPollEvents();
		}


		glfwTerminate();
	}

	Window* Application::GetWindow()
	{
		return m_currentWindow;
	}



}
