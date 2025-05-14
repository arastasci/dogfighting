#pragma once

#include "Entity.h" 
#include "at/utils/Helpers.h"

namespace at
{
	class Scene;
	struct Component
	{
		void SetEntity(Entity e)
		{
			m_Entity = e;

		}
		Entity m_Entity;
	};
}
