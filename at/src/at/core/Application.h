#pragma once

#include "at/core/Core.h"
#include "Window.h"
#include "at/ecs/Scene.h"
namespace at
{
	class AT_API Application
	{
	public:
		static Application* Instance();
		Application();
		virtual ~Application();
		void Init();
		void Run();
		Window* GetWindow();
		virtual void Test() = 0;
		void Update();
	private:
		static Application* m_instance;
		Window* m_currentWindow;

		double m_DeltaTime;
		double m_FrameTime;
		Scene* m_activeScene;
	};
}