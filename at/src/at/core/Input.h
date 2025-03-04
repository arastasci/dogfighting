#pragma once

#include "Core.h"
#include <atpch.h>
#include "KeyCodes.h"
namespace Input
{

	
	AT_API bool GetKeyPress(int keyCode);
	AT_API bool GetKeyRelease(int keyCode);
	AT_API std::pair<float, float> GetMousePos();
	AT_API bool GetMouseButtonPress(int mouseButton);
	AT_API bool GetMouseButtonRelease(int mouseButton);

}