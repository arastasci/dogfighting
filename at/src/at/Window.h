#pragma once
#include <glad/glad.h> 
#include "GLFW/glfw3.h"


namespace at
{

	class  Window
	{
	public:
		Window();
		~Window();
		GLFWwindow* GetGLFWWindow();
	private:
		void Init();

		GLFWwindow* m_GLFWwindow;
		// TODO: callbacks
		friend class Application;
		void SwapBuffers();
		bool ShouldClose();

	};
}