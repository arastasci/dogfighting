#pragma once
#include "atpch.h"
#include "entt/meta/meta.hpp"
namespace at
{
	class Entity;
	template<typename T>
	class Prefab
	{
	public:

		virtual void InitEntity(Entity e) = 0;

	protected:
		Prefab()
		{
			//entt::meta<T>()
			//	.type(entt::hashed_string(entt::type_name<T>().value()));
		}

	};
}