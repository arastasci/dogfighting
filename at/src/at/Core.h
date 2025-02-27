#pragma once

#ifdef AT_CORE
	#define AT_API __declspec(dllexport)
#else
	#define AT_API __declspec(dllimport)
#endif
