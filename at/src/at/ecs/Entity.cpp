#include "Entity.h"

namespace at
{

	Entity::Entity(entt::entity handle, std::shared_ptr<Scene> scene) : m_handle(handle), m_scene(scene)
	{
	}

	Entity::Entity(entt::entity handle, Scene* scene) : m_handle(handle), m_scene(scene)
	{
	}

}