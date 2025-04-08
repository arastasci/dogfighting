#pragma once
#include <atpch.h>
#include "Core.h"
#include <at/ecs/Scene.h>

namespace at
{
	class AT_API Layer
	{
	public:
		Layer(const char* name = "Layer") {};
		~Layer() {};

		virtual void OnUpdate(float dt) {};
		virtual void OnPushed() {};
		virtual void OnPopped() {};

		void SetActiveScene(std::shared_ptr<Scene> scene)
		{
			m_activeScene = scene;
		}

	protected:
		std::shared_ptr<Scene> m_activeScene;
		std::string m_Name;
	};
}