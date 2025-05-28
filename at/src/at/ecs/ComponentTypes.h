#pragma once
#include "atpch.h"
#include <cstddef>
#include <tuple>
#include "CoreComponents/CoreComponents.h"
#include "entt/meta/meta.hpp"
namespace at
{
	using NetworkComponents = std::tuple<
		Transform,
		MeshRenderer
	>;

	template<typename Comp>
	decltype(auto) reflectComponent() {
		return;
	}




}