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

		void RegisterPostSystem(SharedPtr<ISystem> system)
		{
			m_PostSystems.push_back(system);
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

		void Render(double dt)
		{
			for (auto& s : m_Systems)
			{
				s->Render(dt);
			}
		}

		void OnDestroy()
		{
			for (auto& s : m_Systems)
			{
				s->OnDestroy();
			}
		}

		void PostUpdate(float dt)
		{
			for (auto& s : m_PostSystems)
			{
				s->Update(dt);
			}
		}

	private:
		std::shared_ptr<Scene> m_Scene;
		std::vector<std::shared_ptr<ISystem>> m_Systems;
		std::vector<SharedPtr<ISystem>> m_PostSystems;
		friend class Scene;

	};

}