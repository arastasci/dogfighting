#include "Application.h"
#include <GLFW/glfw3.h>
#include "Logger.h"
#include "at/renderer/Renderer.h"
#include "at/ecs/CoreSystems/CameraSystem.h"
#include "at/ecs/CoreSystems/MeshRendererSystem.h"
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

	}


	void Application::Run()
	{
		m_FrameTime = glfwGetTime();
		

		while (!m_currentWindow->ShouldClose())
		{

			auto timeNow = glfwGetTime();
			m_DeltaTime = timeNow - m_FrameTime;
			m_FrameTime = timeNow;

			//Update();
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			for (auto layer : m_layerStack)
			{
				layer->OnUpdate(m_DeltaTime);
			}


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

	void Application::PushLayer(Layer* layer)
	{
		m_layerStack.PushLayer(layer);
		layer->SetActiveScene(m_activeScene);
		layer->OnPushed();
	}

	void Application::PopLayer(Layer* layer)
	{
		m_layerStack.PopLayer(layer);
		layer->OnPopped();
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_layerStack.PushOverlay(layer);
		layer->SetActiveScene(m_activeScene);
		layer->OnPushed();
	}

	void Application::PopOverlay(Layer* layer)
	{
		m_layerStack.PopOverlay(layer);
		layer->OnPopped();
	}

}
