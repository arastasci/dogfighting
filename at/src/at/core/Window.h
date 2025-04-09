#pragma once

#include "atpch.h"

namespace at
{

	class  Window
	{
	public:
		Window(int width, int height, std::string name);
		Window();
		~Window();
		GLFWwindow* GetGLFWWindow();
		std::pair<int, int> GetWindowWidth();

		float GetAspectRatio();
	private:
		int m_Width, m_Height;		
		GLFWwindow* m_GLFWwindow;

		friend class Application;
		void Init();
		void SwapBuffers();
		bool ShouldClose();

	};
}