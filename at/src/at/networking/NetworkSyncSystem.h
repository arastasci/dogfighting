
#pragma once

#include "at/core/Core.h"
#include "at/ecs/System.h"
#include "at/physics/Rigidbody.h"
#include "NetworkTag.h"
#include "Networking.h"
#include "at/ecs/CoreComponents/Transform.h"
namespace at
{
    class AT_API NetworkSyncSystem : public System
    {
    public:
        void FixedUpdate() override
        {
            auto& nc = Networking::Get();
            if (nc.IsHost())
            {
                auto& reg = m_Scene->GetRegistry();
                auto& view = reg.view<NetworkTag, Rigidbody, Transform>();
                for (auto& [e, rb, tr] : view.each())
                {
                    if (rb.IsStatic())
                        continue;
                    auto* msg = new Messages::TransformUpdateMessage(e, tr);
                    nc.SendToAllClients(msg, sizeof(*msg));
                }
            }
        }


    private:
        const bool m_Interpolates = false;
    };
}