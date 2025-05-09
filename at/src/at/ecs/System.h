#pragma once
#include "at/core/Core.h"

#include "entt/entt.hpp"
#include <at/ecs/Scene.h>
#include "ISystem.h"
#include "at/utils/Constants.h"

namespace at
{
	class AT_API System : public ISystem
	{
	public:


		virtual void Update(float dt){}
		virtual void FixedUpdate(){}
	protected:

		template<typename... T>
		auto  GetView() 
		{
			return Scene::ActiveScene()->GetAllEntitiesWith<T...>();
		}

		template<typename... T>
		auto  GetGroup()
		{
			return Scene::ActiveScene()->GetAllComponents<T...>();
		}

	};
}