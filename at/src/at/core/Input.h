#pragma once

#include "Core.h"
#include <atpch.h>
#include "KeyCodes.h"
namespace at
{

	class AT_API Input
	{
	public:
		static bool IsKeyPressed(int keyCode);
		static std::pair<float, float> GetMousePos();
	private:

	};
}