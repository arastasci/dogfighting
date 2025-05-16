#pragma once
#include "at/core/Core.h"

#include "at/ecs/Entity.h"

namespace at
{
	struct AT_API BehaviourComponent
	{

		template<typename T, typename = std::enable_if_t < std::is_base_of_v<Entity, T>>>
		static T* Instantiate()
		{
			return Entity::Instantiate<T>();
		}


		template<typename T, typename = std::enable_if_t < std::is_base_of_v<Entity, T>>>
		static T* Instantiate(Transform t)
		{
			return Entity::Instantiate<T>(t);
		}
	};
}