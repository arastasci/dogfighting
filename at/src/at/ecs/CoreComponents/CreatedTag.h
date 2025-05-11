#pragma once

#include "atpch.h"
#include "at/ecs/Component.h"
namespace at
{
	class CreatedTag : public Component
	{
	public:
		CreatedTag() = default;
		int m_{};
	};
}