#include "Input.h"
#include "at/Application.h"
namespace at
{
	bool Input::IsKeyPressed(int keyCode)
	{
		GLFWwindow* window = Application::Instance()->GetWindow()->GetGLFWWindow();
		auto state = glfwGetKey(window, keyCode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}
	std::pair<float, float> Input::GetMousePos()
	{
		GLFWwindow* window = Application::Instance()->GetWindow()->GetGLFWWindow();
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		return std::pair<float, float>(x, y);
	}
}