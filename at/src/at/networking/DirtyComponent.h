#pragma once
#include "atpch.h"
#include "entt/meta/meta.hpp"
namespace at
{
	struct DirtyComponent
	{
		std::unordered_set<size_t> DirtyComponentIDs{};
		
		template<typename T>
		void SetDirty()
		{
			DirtyComponentIDs.insert(entt::resolve<T>().id());
		}

		void Consume()
		{
			DirtyComponentIDs.clear();
		}
	};
}