#pragma once
#include "atpch.h"
#include "at/core/Core.h"
#include "at/core/Logger.h"
#include "at/ecs/Scene.h"
#include "at/core/EngineSubsystem.h"
#include "steam/isteamnetworkingutils.h"
#ifdef USE_STEAM
#include "steam/isteamuser.h"
#include "steam/isteamclient.h"
#include "steam/steam_api.h"
#include "steam/isteammatchmaking.h"
#else
#include "steam/steamnetworkingsockets.h"
#endif
#include "Messages.h"

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
			AT_INFO("SteamID is: {}", SteamUser()->GetSteamID().ConvertToUint64());
			m_selfSteamID = SteamUser()->GetSteamID();
			m_Interface = SteamNetworkingSockets();
			SteamNetworkingUtils()->InitRelayNetworkAccess();
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

		//void UpdateDirtyComponents();
		using ClientConnectedCallback = std::function<void(SharedPtr<Scene>, HSteamNetConnection)>;
		using HandleAppMessageCallback = std::function<void(SharedPtr<Scene>, SteamNetworkingMessage_t*)>;
		using HandleAppServerMessageCallback = std::function<void(SharedPtr<Scene>, HSteamNetConnection, SteamNetworkingMessage_t*)>;
		void HandleAppMessage(SteamNetworkingMessage_t* msg);
		void Update();
		void Host();
		void Connect();
#ifdef USE_STEAM
		void Connect(CSteamID id);
#endif
		void BindScene(SharedPtr<Scene> scene);
		void SetHandleServerAppMessageCallback(HandleAppServerMessageCallback);
		void SetHandleClientAppMessageCallback(HandleAppMessageCallback);
		void SetClientConnectedCallback(ClientConnectedCallback);
		static void OnNetworkedEntityDestroyedCallback(entt::registry& registry, entt::entity e);
		void OnNetworkedEntityDestroyed(entt::registry& registry, entt::entity e);
		static void OnNetworkedEntityCreatedCallback(entt::registry& registry, entt::entity e);
		void OnNetworkedEntityCreated(entt::registry& registry, entt::entity e);
		void SendToHost( void*, size_t size);
		void SendToAllClients(const void*, size_t size);
		void SendToClient(ClientID id, const void*, size_t size);
		bool IsHost();
		bool IsClient();
		bool HostNotAlone() { return m_ConnectedClients.size() > 1; }
		bool IsConnectionHost(HSteamNetConnection conn) { return conn == m_HostConnection; }
		HSteamNetConnection GetHostConnection() { return m_HostConnection; }
		bool IsConnectedToHost() { return m_ClientConnected; }
		void SetConnected(bool b) { m_ClientConnected = b; }
		template<typename T>
		void OnComponentCreated(entt::entity e)
		{
			if (IsHost())
			{
				SendToAllClients(new Messages::ComponentCreatedMessage(e, StaticComponentID<T>::value));
			}
		}
		void ReceiveMessages();
		void SetSelfClientId(HSteamNetConnection conn);
		HSteamNetConnection GetClientId() { return SelfClientID; }
		entt::entity ToLocal(entt::entity);
#ifdef USE_STEAM
		CSteamID GetSteamID() { return m_selfSteamID; };
#endif
	private:
#ifdef USE_STEAM
		CSteamID m_selfSteamID;
#endif

		bool m_ClientConnected = false;
		HSteamNetConnection SelfClientID;
		HandleAppServerMessageCallback m_HandleServerAppMessageCallback;
		HandleAppMessageCallback m_HandleClientAppMessageCallback;
		ClientConnectedCallback m_ClientConnectedCallback;
		void OnComponentCreated(entt::entity, size_t);
		static void ConnectionStatusChangedCallbackHost(SteamNetConnectionStatusChangedCallback_t* info);
		void OnConnectionStatusChangedHost(SteamNetConnectionStatusChangedCallback_t* info);
		static void ConnectionStatusChangedCallbackClient(SteamNetConnectionStatusChangedCallback_t* info);
		void OnConnectionStatusChangedClient(SteamNetConnectionStatusChangedCallback_t* info);
		bool m_IsClient;
		bool m_IsHost;
		std::unordered_set<ClientID> m_ConnectedClients;
		std::unordered_map<entt::entity, entt::entity> m_RemoteToLocalMap;
		std::unordered_map<entt::entity, entt::entity> m_LocalToRemoteMap;
		SharedPtr<Scene> m_Scene;
		const int m_DefaultPort = 40002;
		ISteamNetworkingSockets* m_Interface = nullptr;
		HSteamNetConnection m_Connection = 0;
		HSteamNetConnection m_HostConnection;
		HSteamListenSocket m_ListenSocket;
		HSteamNetPollGroup m_PollGroup = 0u;
	};
}