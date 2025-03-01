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
	private:
		void Init();

		GLFWwindow* m_window;
		// TODO: callbacks
		friend class Application;

		bool ShouldClose();

	};
}