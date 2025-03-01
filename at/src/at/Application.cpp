#include "Application.h"
#include <GLFW/glfw3.h>
#include "Logger.h"
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
	
	void Application::Run()
	{
		while (!m_currentWindow->ShouldClose())
		{


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
