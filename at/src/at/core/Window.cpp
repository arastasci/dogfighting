#include "Window.h"
#include <atpch.h>

namespace at
{

	Window::Window(int width, int height, std::string name) 
		: m_Width(width), m_Height(height)
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

		Init();
	}

	


	Window::Window() : Window(1280, 720, "at")
	{
		
	}
	Window::~Window()
	{
	}

	void Window::Init()
	{
		// TODO: set event callbacks
	}

	GLFWwindow* Window::GetGLFWWindow()
	{
		return m_GLFWwindow;
	}

	std::pair<int, int> Window::GetWindowWidth()
	{
		return std::pair<int, int>(m_Width, m_Height);
	}

	float Window::GetAspectRatio()
	{
		return m_Width / (float) m_Height;
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

