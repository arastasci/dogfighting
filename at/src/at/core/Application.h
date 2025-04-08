#pragma once

#include "at/core/Core.h"
#include "Window.h"
#include "at/ecs/Scene.h"
#include "LayerStack.h"
#include "GameLayer.h"
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
		void Update();

		void PushLayer(Layer* layer);
		void PopLayer(Layer* layer);
		void PushOverlay(Layer* layer);
		void PopOverlay(Layer* layer);
	protected:
		static Application* m_instance;
		Window* m_currentWindow;
		LayerStack m_layerStack;
		double m_DeltaTime;
		double m_FrameTime;
		GameLayer m_GameLayer;
		std::shared_ptr<Scene> m_activeScene;
	};
}