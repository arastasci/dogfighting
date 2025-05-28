#pragma once
#include "atpch.h"
#include "entt/entt.hpp"

namespace at
{
	template<typename Derived>
	struct IComponent
	{
		virtual void Register() = 0;
		virtual void Serialize(const void* buffer)
		{
			(*static_cast<Derived*>(this)).Serialize(buffer);
		}
	};
}