#pragma once

#include "Core.h"

namespace at
{
	class AT_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};
}