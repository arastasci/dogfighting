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
	void Window::Init()
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		m_window = glfwCreateWindow(1280, 720, "at", nullptr, nullptr);
		glfwMakeContextCurrent(m_window);

		if (m_window == NULL)
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
	bool Window::ShouldClose()
	{
		return glfwWindowShouldClose(m_window);
	}
}

