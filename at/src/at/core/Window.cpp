#include "Window.h"
#include <atpch.h>

namespace at
{
	Window::Window()
	{
	}
	Window::~Window()
	{
	}

	GLFWwindow* Window::GetGLFWWindow()
	{
		return m_GLFWwindow;
	}

	void Window::Init()
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		m_GLFWwindow = glfwCreateWindow(1280, 720, "at", nullptr, nullptr);
		glfwMakeContextCurrent(m_GLFWwindow);

		if (m_GLFWwindow == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return;
		}

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			return;
		}

		glViewport(0, 0, 1280, 720);
	}
	void Window::SwapBuffers()
	{
		glfwSwapBuffers(m_GLFWwindow);
	}
	bool Window::ShouldClose()
	{
		return glfwWindowShouldClose(m_GLFWwindow);
	}
}

