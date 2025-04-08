#pragma once
#include <atpch.h>
#include "Core.h"
#include "Layer.h"
#include <at/ecs/CoreSystems/CameraSystem.h>
#include <at/ecs/CoreSystems/MeshRendererSystem.h>
namespace at
{
	class AT_API GameLayer : public Layer
	{
	public:

		virtual void GameInit() {};

		void OnPushed() override final
		{
			std::shared_ptr<CameraSystem> cameraSystem = std::make_shared<CameraSystem>();
			std::shared_ptr<MeshRendererSystem> mrSystem = std::make_shared<MeshRendererSystem>();
			m_activeScene->m_SystemScheduler->Register(cameraSystem);
			m_activeScene->m_SystemScheduler->Register(mrSystem);

			GameInit(); // application initialization code
		}

		void OnUpdate(float dt) override
		{
			m_activeScene->m_SystemScheduler->Update(dt);
		}
	};
}