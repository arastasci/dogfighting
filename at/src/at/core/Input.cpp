#include "Input.h"
#include "at/core/Application.h"
#include "at/core/Window.h"
namespace Input
{

	static CursorMode g_cursorMode = CursorMode::Normal;


	void SetCursorMode(CursorMode mode)
	{

		auto* glfwWindow = at::Application::Instance()->GetWindow()->GetGLFWWindow();
		switch (mode)
		{
			case CursorMode::Locked:
			{
				glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				break;
			}
			case CursorMode::Hidden:
			{
				glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
				break;
			}
			case CursorMode::Normal:
			{
				glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				break;
			}
		}
	}
	CursorMode GetCursorMode()
	{
		return g_cursorMode;
	}
	bool GetKeyPress(int keyCode)
	{
		GLFWwindow* window = at::Application::Instance()->GetWindow()->GetGLFWWindow();
		auto state = glfwGetKey(window, keyCode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
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