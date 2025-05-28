#include "Application.h"
#include <GLFW/glfw3.h>
#include "Logger.h"
#include "at/renderer/Renderer.h"
#include "at/networking/Networking.h"
#include "at/utils/Constants.h"

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


		RegisterComponents();
		ModelLibrary::Get().Init();
		MaterialLibrary::Get().Init();
		ShaderLibrary::Get().Init();
		Renderer::Init();
		Networking::Get().Init();
		char c;
		std::cout << "h or c" << std::endl;
		std::cin >> c;
		if (c == 'h')
		{
			Networking::Get().Host();
			Networking::Get().Connect();
		}
		else
		{
			Networking::Get().Connect();
		}

		m_activeScene->Init();
		
		Networking::Get().BindScene(m_activeScene);

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
			Networking::Get().Update();
			m_activeScene->Start();

			m_activeScene->PreUpdate();
			const auto accTime = m_activeScene->FixedUpdate(m_DeltaTime);
			m_activeScene->Update(m_DeltaTime);
			m_activeScene->InterpolatePhysicsTransforms(accTime / Constants::FIXED_TIMESTEP);
			m_activeScene->LateUpdate(m_DeltaTime);
			m_activeScene->OnDestroy();
			m_activeScene->Render();
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

	void Application::RegisterComponents()
	{
		reflectComponent<Transform>();
	}



}
