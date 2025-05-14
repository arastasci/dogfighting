#include "Entity.h"
#include "at/ecs/CoreComponents/ToBeDestroyedTag.h"
namespace at
{

	Entity::Entity(entt::entity handle, std::shared_ptr<Scene> scene) : m_handle(handle), m_scene(scene)
	{
	}



	void Entity::DestroyEntity()
	{
		if(!HasComponent<ToBeDestroyedTag>())
			AddComponent<ToBeDestroyedTag>();
	}

}