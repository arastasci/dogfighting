#pragma once

#include "atpch.h"
#include "at/core/Core.h"
#include "at/ecs/CoreComponents/CreatedTag.h"
#include "at/ecs/CoreComponents/ActiveTag.h"
#include "at/ecs/System.h"
namespace at
{
	/// <summary>
	/// Executed first in the order of systems.
	/// </summary>
	class AT_API EntityManagerSystem : public System
	{
	public:
		virtual void Update(float dt) override
		{
			auto view = GetStartedView<>();

			for (auto [e_] : view.each())
			{
				Entity e(e_, m_Scene);
				e.RemoveComponent<CreatedTag>();
				e.AddComponent<ActiveTag>();
			}
		}


	};
}