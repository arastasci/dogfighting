#pragma once
#include "at/core/Core.h"

#include "entt/entt.hpp"
#include <at/ecs/Scene.h>
#include "ISystem.h"
namespace at
{
	template<typename... Components>
	class AT_API System : public ISystem
	{
	public:


		virtual void Update(float dt){	}

	protected:

		template<typename... T>
		auto  GetView() 
		{
			return Scene::ActiveScene()->GetAllEntitiesWith<T...>();
		}

		auto  GetView()
		{
			return Scene::ActiveScene()->GetAllEntitiesWith<Components...>();
		}

		auto  GetGroup()
		{
			return Scene::ActiveScene()->GetAllComponents<Components...>();
		}

	};
}