#pragma once

#include "atpch.h"
#include "at/ecs/ISystem.h"
#include "at/utils/Constants.h"
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

		void Update(float dt)
		{
			for (auto& s : m_Systems)
			{
				s->Update(dt);
			}
		}

		void FixedUpdate(double dt)
		{
			while (dt >= Constants::FIXED_TIMESTEP)
			{
				for (auto& s : m_Systems)
				{
					s->FixedUpdate();
				}
				dt -= Constants::FIXED_TIMESTEP;
			}
		}

	private:
		std::shared_ptr<Scene> m_Scene;
		std::vector<std::shared_ptr<ISystem>> m_Systems;

		friend class Scene;

	};

}