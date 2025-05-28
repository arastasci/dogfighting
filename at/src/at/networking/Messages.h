#pragma once
#include "atpch.h"
#include "entt/entt.hpp"
#include "at/ecs/CoreComponents/CoreComponents.h"
#include "at/ecs/ComponentTypes.h"

namespace Messages
{
	using namespace at;
	enum  MessageType
	{
		EntityCreated,
		EntityDestroyed,
		TransformUpdate,
		CoreMessageCount,
			
	};

	template<uint16_t type>
	struct Message
	{
		MessageType m_Type = static_cast<MessageType>(type);
	};


	struct EntityCreatedMessage :  public Message<EntityCreated>
	{
		EntityCreatedMessage(entt::entity e_) : e(e_) {}
		entt::entity e;
	};

	struct EntityDestroyedMessage : public Message<EntityDestroyed>
	{
		EntityDestroyedMessage(entt::entity e_) : e(e_) {}
		entt::entity e;
	};

	struct TransformUpdateMessage : public Message<TransformUpdate>
	{
		TransformUpdateMessage(entt::entity e_, Transform t) : e(e_), transform(t) {}
		entt::entity e;
		Transform transform;
	};


}
