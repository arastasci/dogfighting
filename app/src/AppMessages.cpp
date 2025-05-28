#include "AppMessages.h"
#include "PlaneController.h"
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
        auto planeHandle = PlaneControllerSystem::m_ConnToEntityMap[conn];
        auto& planeRb = reg.get<Rigidbody>(planeHandle);
        auto& t = reg.get<Transform>(planeHandle);
        auto handle = static_cast<entt::entity>(scene->CreateNetworkedPrefab<RocketPrefab>(Transform(t.position - 2.25f * t.Up() + 5.25f * t.Forward(), PlaneControllerSystem::RotateRocket(t), vec3(0.01f))));
        auto& rocketBehaviour = scene->GetRegistry().get<RocketBehaviour>(handle);
        rocketBehaviour.initialVelocity = planeRb.GetRigidbody().get()->getLinearVelocity();

        scene->GetRegistry().get<RocketBehaviour>(handle).owner = conn;
        break;
    }


    }
};

void Messages::HandleAppMessages_Client(SharedPtr<Scene> scene, SteamNetworkingMessage_t* incomingMessage)
{
    auto& nc = Networking::Get();
    Messages::AppMessageType_Client e = *(static_cast<const Messages::AppMessageType_Client*>((incomingMessage->GetData())));

    switch (e)
    {
    case PlayerSpawned:
    {
        auto msg = *static_cast<const PlayerSpawnedMessage*>((incomingMessage->GetData()));
        auto localHandle = nc.ToLocal(msg.e);
        Entity entity{ localHandle, scene };
        auto camera = scene->CreateEntity(Transform(&entity.GetComponent<Transform>(), vec3(-2.5f, 2.5f, -6.0f), quat(), vec3(1.0f)));

        camera.AddComponent<CameraComponent>(camera.GetComponent<Transform>().position, Vector3::forward, Vector3::up, 45.0f, 1280.f / 720.f);
        //camera.AddComponent<FreeCamera>();

        break;
    }
    }

};
