#include "Input.h"
#include "at/core/Application.h"
namespace Input
{
	bool GetKeyPress(int keyCode)
	{
		GLFWwindow* window = at::Application::Instance()->GetWindow()->GetGLFWWindow();
		auto state = glfwGetKey(window, keyCode);
		return state == GLFW_PRESS;
	}
	bool GetKeyRelease(int keyCode)
	{
		GLFWwindow* window = at::Application::Instance()->GetWindow()->GetGLFWWindow();
		auto state = glfwGetKey(window, keyCode);
		return state == GLFW_RELEASE;
	}
	std::pair<float, float> GetMousePos()
	{
		GLFWwindow* window = at::Application::Instance()->GetWindow()->GetGLFWWindow();
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		return std::pair<float, float>(x, y);
	}
	bool GetMouseButtonPress(int button)
	{
		GLFWwindow* window = at::Application::Instance()->GetWindow()->GetGLFWWindow();
		auto state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS;
	}
	bool GetMouseButtonRelease(int button)
	{
		GLFWwindow* window = at::Application::Instance()->GetWindow()->GetGLFWWindow();
		auto state = glfwGetMouseButton(window, button);
		return state == GLFW_RELEASE;
	}
}