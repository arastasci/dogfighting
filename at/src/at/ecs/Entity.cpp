#include "Entity.h"

namespace at
{

	Entity::Entity(entt::entity handle, Scene* scene) : m_handle(handle), m_scene(scene)
	{
	}

}