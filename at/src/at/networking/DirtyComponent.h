#pragma once
#include "atpch.h"
#include "entt/entt.hpp"
namespace at
{
	struct DirtyComponent
	{
		std::unordered_set<entt::type_info> DirtyComponentIDs{};
	};
}