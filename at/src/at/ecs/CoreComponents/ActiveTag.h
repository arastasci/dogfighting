#pragma once
#include "atpch.h"
#include "at/ecs/Component.h"

namespace at
{
	class ActiveTag : public Component
	{
	public:
		ActiveTag() = default;
		int m_{};
	};
}