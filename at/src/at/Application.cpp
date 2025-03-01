#include "Application.h"
#include <GLFW/glfw3.h>
namespace at {
	Application::Application()
	{
		
	}

	Application::~Application()
	{

	}

	void Application::Init()
	{
		m_currentWindow = new Window();
		m_currentWindow->Init();
	}
	
	void Application::Run()
	{
		while (!m_currentWindow->ShouldClose())
		{
			glfwSwapBuffers(m_currentWindow->m_window);
			glfwPollEvents();
		}
	}

}
