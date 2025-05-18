#pragma once
#include "atpch.h"
#include "entt/entt.hpp"
namespace at
{
	namespace Messages
	{
		enum  MessageType
		{
			EntityCreated,
			EntityDestroyed,
		};

		template<MessageType type>
		struct Message
		{
			MessageType m_Type = type;
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
	}
}