#include "AppMessages.h"
#include "PlaneController.h"
#include "FreeCamera.h"
void Messages::HandleAppMessages_Server(SharedPtr<Scene> scene, HSteamNetConnection conn, SteamNetworkingMessage_t* incomingMessage)
{
    auto& nc = Networking::Get();
    auto& reg = scene->GetRegistry();
    Messages::AppMessageType_Server e = *(static_cast<const Messages::AppMessageType_Server*>((incomingMessage->GetData())));

    switch (e)
    {
    case RocketFired:
    {
        auto msg = *static_cast<const RocketFiredMessage*>((incomingMessage->GetData()));
        auto planeHandle = PlaneFlightSystem::m_ConnToEntityMap[incomingMessage->m_conn];
        auto& planeRb = reg.get<Rigidbody>(planeHandle);
        auto& t = reg.get<Transform>(planeHandle);
        auto handle = static_cast<entt::entity>(scene->CreateNetworkedPrefab("rocket", Transform(t.position - 2.25f * t.Up() + 5.25f * t.Forward(), PlaneFlightSystem::RotateRocket(t), vec3(0.01f))));
        auto& rocketBehaviour = scene->GetRegistry().get<RocketBehaviour>(handle);
        rocketBehaviour.initialVelocity = planeRb.GetRigidbody().get()->getLinearVelocity();

        scene->GetRegistry().get<RocketBehaviour>(handle).owner = incomingMessage->m_conn;
        break;
    }
    case PlayerInput:
    {
        auto msg = *static_cast<const PlaneInputMessage*>((incomingMessage->GetData()));

        auto planeHandle = PlaneFlightSystem::m_ConnToEntityMap[conn];
        auto& controller = reg.get<PlayerPlaneController>(planeHandle);
        controller = msg.inputPayload;

        //if (nc.IsConnectionHost(conn))
        //{
        //AT_INFO("pitch {}, roll {}, shooting {}", controller.pitch, controller.roll, controller.shooting);
        //}
        break;
    }
    case ConnectionApproved:
    {
        if (!nc.IsConnectionHost(conn))
        {
            auto entity = scene->CreateNetworkedPrefab("plane", Transform(vec3(60, 0, 60))); // host

            auto handle = static_cast<entt::entity>(entity);
            PlaneFlightSystem::m_ConnToEntityMap[nc.GetHostConnection()] = handle;
            auto* msg = new Messages::PlayerSpawnedMessage(handle);
            nc.SendToClient(nc.GetHostConnection(), msg, sizeof(*msg));

            auto clientEntity = scene->CreateNetworkedPrefab("plane", Transform(vec3(-20, 0, 60))); // client
            auto clientHandle = static_cast<entt::entity>(clientEntity);
            PlaneFlightSystem::m_ConnToEntityMap[conn] = clientHandle;
            //AT_INFO("Conn {}, Handle {}", conn, handle);
            auto* clientMsg = new Messages::PlayerSpawnedMessage(clientHandle);
            nc.SendToClient(conn, clientMsg, sizeof(*clientMsg));

        }
        break;
    }
    }
};


void Messages::HandleAppMessages_Client(SharedPtr<Scene> scene, SteamNetworkingMessage_t* incomingMessage)
{
    auto& nc = Networking::Get();
    auto& reg = scene->GetRegistry();
    Messages::AppMessageType_Client e = *(static_cast<const Messages::AppMessageType_Client*>((incomingMessage->GetData())));

    switch (e)
    {
    case PlayerSpawned:
    {
        Networking::Get().SetConnected(true);
        auto msg = *static_cast<const PlayerSpawnedMessage*>((incomingMessage->GetData()));
        auto localHandle = nc.ToLocal(msg.e);
        Entity entity{ localHandle, scene };
        auto& ppc = reg.get<PlayerPlaneController>(localHandle);
        ppc.IsOwned = true;
        auto camera = scene->CreateEntity(Transform(&entity.GetComponent<Transform>(), vec3(-2.5f, 2.5f, -6.0f), quat(), vec3(1.0f)));
        camera.AddComponent<CameraComponent>(camera.GetComponent<Transform>().position, Vector3::forward, Vector3::up, 45.0f, 1280.f / 720.f);
        camera.AddComponent<FreeCamera>();

        break;
    }
    case Connected:
    {
        auto msg = *static_cast<const ConnectedMessage*>((incomingMessage->GetData()));
        nc.SetSelfClientId(msg.conn);
        auto* approvedMsg = new ConnectionApprovedMessage();
        nc.SendToHost(approvedMsg, sizeof(*approvedMsg));
        break;
    }
    }

};
