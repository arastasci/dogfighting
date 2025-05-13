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
		virtual void AppInit() = 0;
		void Init();
		void Run();
		Window* GetWindow();

	protected:
		static Application* m_instance;
		Window* m_currentWindow;
		double m_AccumulatedTime;
		double m_DeltaTime;
		double m_FixedDeltaTime;
		double m_LastFixedTime;
		double m_FrameTime;
		std::shared_ptr<Scene> m_activeScene;
	};
}