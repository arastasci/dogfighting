#pragma once

#include "Core.h"
#include "Window.h"
namespace at
{
	class AT_API Application
	{
	public:
		Application();
		virtual ~Application();
		void Init();
		void Run();
	private:
		Window* m_currentWindow;
	};
}