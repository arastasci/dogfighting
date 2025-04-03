#pragma once

#include "Entity.h" 

namespace at
{
	struct Component
	{
		void SetEntity(Entity e)
		{
			m_Entity = e;
		}

		Entity m_Entity;
	};
}
