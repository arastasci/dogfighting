#include "Application.h"
#include <GLFW/glfw3.h>
#include "Logger.h"
#include "at/renderer/Renderer.h"
#include "at/networking/Networking.h"
#include "at/ecs/CoreSystems/EntityManagerSystem.h"
#include "at/ecs/CoreSystems/CameraSystem.h"
#include "at/ecs/CoreSystems/MeshRendererSystem.h"
#include "at/ecs/CoreSystems/PointLightSystem.h"
#include "at/physics/PhysicsSystem.h"
#include "at/renderer/SkyboxSystem.h"
#include "at/utils/Constants.h"

#include "Input.h"
#include <at/networking/NetworkingSystem.h>

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

		char option;
		Logger::GetCoreLogger()->info("h or c?");
		std::cin >> option;
		Networking::Get().Init();

		if (option == 'h')
		{
			Networking::Get().Host();
		}
		else
		{
			Networking::Get().Connect();
		}


		m_activeScene->Init();


		m_activeScene->AddSystem<EntityManagerSystem>();
		m_activeScene->AddSystem<PhysicsSystem>();
		m_activeScene->AddSystem<NetworkingSystem>();
		m_activeScene->AddPostSystem<CameraSystem>();
		m_activeScene->AddPostSystem<MeshRendererSystem>();
		m_activeScene->AddPostSystem<PointLightSystem>();
		m_activeScene->AddPostSystem<SkyboxSystem>();
		
		stbi_set_flip_vertically_on_load(true);
		AppInit(); // application initialization code
	}


	void Application::Run()
	{
		m_FrameTime = glfwGetTime();
		m_LastFixedTime = m_FrameTime;
		while (!m_currentWindow->ShouldClose())
		{
			auto timeNow = glfwGetTime();

			m_DeltaTime = timeNow - m_FrameTime;
			m_FrameTime = timeNow;

			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			//////////////    UPDATE         ///////////////

			m_activeScene->Start();

			auto accTime = m_activeScene->FixedUpdate(m_DeltaTime);

			m_activeScene->Update(m_DeltaTime);
			m_activeScene->Render(accTime / Constants::FIXED_TIMESTEP);
			m_activeScene->PostUpdate(m_DeltaTime);
			m_activeScene->EndFrame();
			
			//////////////    UPDATE - END         ///////////////


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
