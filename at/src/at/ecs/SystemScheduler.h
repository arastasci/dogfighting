#pragma once

#include "atpch.h"
#include "at/ecs/ISystem.h"
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

	private:
		std::shared_ptr<Scene> m_Scene;
		std::vector<std::shared_ptr<ISystem>> m_Systems;

		friend class Scene;

	};

}