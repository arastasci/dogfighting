#pragma once
#include "atpch.h"
#include "at/ecs/Component.h"
namespace at
{
	class ToBeDestroyedTag : public Component
	{
	public:
		ToBeDestroyedTag() = default;
	private:
		int m_{};
	};
}