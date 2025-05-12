#pragma once

#include "atpch.h"


namespace at
{
	class Scene;

	class ISystem
	{
	public:
		//virtual void Init() = 0;
		virtual void Start() = 0;
		virtual void Update(float deltaTime) = 0;
		virtual void Render(float deltaTime) = 0;
		virtual void FixedUpdate() = 0;
		virtual void OnDestroy() = 0;
		void SetScene(std::shared_ptr<Scene> scene) { m_Scene = scene; }
		
	protected:
		std::shared_ptr<Scene> m_Scene;
	};
}