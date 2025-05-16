#pragma once

#include "atpch.h"
#include "at/ecs/ISystem.h"
#include "at/utils/Constants.h"
#include "at/utils/Helpers.h"
namespace at
{
	class SystemScheduler
	{
	public:
		SystemScheduler() = default;

		void Register(std::shared_ptr<ISystem> system)
		{
			m_Systems.push_back(system);
			system->SetScene(m_Scene);
		}


		void Start()
		{
			for (auto& s : m_Systems)
			{
				s->Start();
			}
		}

		void Update(float dt)
		{
			for (auto& s : m_Systems)
			{
				s->Update(dt);
			}
		}

		void FixedUpdate(double dt)
		{
				for (auto& s : m_Systems)
				{
					s->FixedUpdate();
				}
		}


		void OnDestroy()
		{
			for (auto& s : m_Systems)
			{
				s->OnDestroy();
			}
		}

		void LateUpdate(float dt)
		{
			for (auto& s : m_Systems)
			{
				s->LateUpdate(dt);
			}
		}

	private:
		std::shared_ptr<Scene> m_Scene;
		std::vector<std::shared_ptr<ISystem>> m_Systems;
		friend class Scene;

	};

}