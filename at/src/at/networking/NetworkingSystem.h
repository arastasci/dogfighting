// at/ecs/System/NetworkingSystem.h
#pragma once
#include "atpch.h"
#include "at/ecs/System.h"
#include "at/networking/Networking.h"
#include "at/ecs/CoreComponents/Transform.h"
#include <entt/entity/registry.hpp>

namespace at
{
	class NetworkingSystem : public System
	{
	public:
		void Update(float dt) override
		{
			auto& net = Networking::Get();

			if (net.IsClient())
			{
				net.ReceiveNetworkData();

				std::vector<TransformUpdateMessage> updates;
				net.GetTransformUpdates(updates);

				auto& registry = m_Scene->GetRegistry();
				for (const auto& u : updates)
				{
					entt::entity e = static_cast<entt::entity>(u.entity);
					if (registry.valid(e) && registry.all_of<Transform>(e))
					{
						auto& t = registry.get<Transform>(e);
						t.position = u.position;
						t.rotation = u.rotation;
						t.scale = u.scale;
					}
				}
			}
			net.RunCallback();
		}

		void FixedUpdate() override
		{
			auto& net = Networking::Get();
			if (!net.IsHost()) return;

			auto view = GetView<Rigidbody, Transform>();
			for (auto [e, _, rb, t] : view.each())
			{
				net.SendTransformUpdate(static_cast<uint32_t>(e), t);
			}
		}
	};
}
