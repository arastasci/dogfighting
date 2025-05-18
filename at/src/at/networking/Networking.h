#pragma once
#include "atpch.h"
#include "at/core/Core.h"
#include "at/core/Logger.h"
#include "at/ecs/Scene.h"
#include "at/core/EngineSubsystem.h"
#include "steam/isteamnetworkingutils.h"
#include "steam/steamnetworkingsockets.h"
#ifndef STEAMNETWORKINGSOCKETS_OPENSOURCE
#include "steam/isteamuser.h"
#include "steam/isteamclient.h"
#include "steam/steam_api.h"
#endif
namespace at
{
	using ClientID = HSteamNetConnection;
	class AT_API Networking : public EngineSubsystem<Networking>
	{
	public:
		virtual void Init() override
		{
#ifdef USE_STEAM
			SteamErrMsg errMsg = { 0 };
			if (SteamAPI_InitEx(&errMsg) != k_ESteamAPIInitResult_OK)
			{
				AT_CORE_WARN("SteamAPI_Init() failed: ");
				AT_CORE_WARN(errMsg);
				AT_CORE_WARN("\n");

				AT_CORE_CRITICAL("Fatal Error", "Steam must be running to play this game (SteamAPI_Init() failed).\n");
				assert(false);
				return;
			}
			SteamUser()->GetSteamID();
#else 
			SteamDatagramErrMsg errMsg;
			if (!GameNetworkingSockets_Init(nullptr, errMsg))
			{
				return;
			}
			m_Interface = SteamNetworkingSockets();
#endif
		}

		virtual void Shutdown() override
		{

		}

		void Update();
		void Host();
		void Connect();
		void BindRegistry(entt::registry& registry);
		static void OnNetworkedEntityDestroyedCallback(entt::registry& registry, entt::entity e);
		void OnNetworkedEntityDestroyed(entt::registry& registry, entt::entity e);
		static void OnNetworkedEntityCreatedCallback(entt::registry& registry, entt::entity e);
		void OnNetworkedEntityCreated(entt::registry& registry, entt::entity e);
		void SendToHost(const void*);
		void SendToAllClients(const void*);
		void SendToClient(ClientID id, const void*);
		bool IsHost();
		bool IsClient();

		void ReceiveMessages();
	private:
		static void ConnectionStatusChangedCallbackHost(SteamNetConnectionStatusChangedCallback_t* info);
		void OnConnectionStatusChangedHost(SteamNetConnectionStatusChangedCallback_t* info);
		static void ConnectionStatusChangedCallbackClient(SteamNetConnectionStatusChangedCallback_t* info);
		void OnConnectionStatusChangedClient(SteamNetConnectionStatusChangedCallback_t* info);
		bool m_IsClient;
		bool m_IsHost;
		std::unordered_set<ClientID> m_ConnectedClients;
		std::unordered_map<entt::entity, entt::entity> m_RemoteEntityMap;
		const int m_DefaultPort = 40002;
		ISteamNetworkingSockets* m_Interface = nullptr;
		HSteamNetConnection m_Connection = 0;
		HSteamListenSocket m_ListenSocket;
		HSteamNetPollGroup m_PollGroup = 0u;
	};
}