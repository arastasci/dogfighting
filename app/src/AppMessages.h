#pragma once
#include "at.h"

namespace Messages
{
    enum AppMessageType_Server
    {
        RocketFired = MessageType::CoreMessageCount + 1,
        PlayerInput,
        ServerMessageCount,
    };



    struct RocketFiredMessage : public Message<RocketFired>
    {

    };

    enum AppMessageType_Client
    {
        PlayerSpawned = ServerMessageCount + 1, // the client that this message sent to itself spawned, and the entity handle is the plane it owns
        PlayerDead,
    };

    struct PlayerSpawnedMessage : public Message<PlayerSpawned>
    {
        PlayerSpawnedMessage(entt::entity e_) : e(e_) {}
        entt::entity e;
    };

    


     void HandleAppMessages_Server(SharedPtr<Scene> scene, HSteamNetConnection conn, SteamNetworkingMessage_t* incomingMessage);

     void HandleAppMessages_Client(SharedPtr<Scene> scene, SteamNetworkingMessage_t* incomingMessage);
}