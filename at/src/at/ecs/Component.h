#pragma once

#include "Entity.h" 
#include "at/utils/Helpers.h"

namespace at
{
	class Scene;
	struct Component
	{
		void SetScene(SharedPtr<Scene> s)
		{
			m_Scene = s;
		}

		void SetEntity(Entity e)
		{
			m_Entity = e;
		}
		SharedPtr<Scene> m_Scene;
		Entity m_Entity;
	};
}
