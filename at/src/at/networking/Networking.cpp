#include "Networking.h"
#include "at/core/Logger.h"
#include "NetworkTag.h"
#include "at/ecs/Entity.h"
#include "DirtyComponent.h"
#include "entt/core/hashed_string.hpp"
namespace at
{
	//void Networking::UpdateDirtyComponents()
	//{

	//	auto& reg = m_Scene->GetRegistry();
	//	auto view = reg.view<ActiveTag, DirtyComponent>();

	//	for (auto& [e, dc] : view.each())
	//	{
	//		auto& dirtyComponents = dc.DirtyComponentIDs;

	void Networking::HandleAppMessage(SteamNetworkingMessage_t* msg)
	{

	}

	//		for (auto& id : dirtyComponents)
	//		{
	//			auto mType = entt::resolve(id);
	//			auto getFunc = mType.func(entt::hashed_string("get"));
	//			entt::meta_any comp = getFunc.invoke(reg, e);
	//			SendToAllClients(new Messages::ComponentUpdatedMessage(e, id, comp.base().data()));
	//		}
	//		dc.Consume();
	//	}
	//}
	void Networking::Update()
	{
		
		ReceiveMessages();
		m_Interface->RunCallbacks();
	}

	void Networking::Host()
	{
		SteamNetworkingIPAddr addr;
		addr.Clear();
		addr.m_port = m_DefaultPort;
		SteamNetworkingConfigValue_t options;
		options.SetPtr(k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged, (void*)ConnectionStatusChangedCallbackHost);

#ifdef USE_STEAM
		m_ListenSocket = m_Interface->CreateListenSocketP2P(0, 1, &options);
#else
		m_ListenSocket = m_Interface->CreateListenSocketIP(addr, 1, &options);
#endif	
		if (m_ListenSocket != k_HSteamListenSocket_Invalid)
		{
			m_IsHost = true;
		}
	}

	void Networking::Connect()
	{
		{
			SteamNetworkingIPAddr addr;
			if (!addr.ParseString("127.0.0.1"))
			{
				return;
			}
			addr.m_port = m_DefaultPort;
			SteamNetworkingConfigValue_t options;
			options.SetPtr(k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged, (void*)ConnectionStatusChangedCallbackClient);
			m_Connection = m_Interface->ConnectByIPAddress(addr, 1, &options);

			if (m_Connection != k_HSteamNetConnection_Invalid)
			{
				m_IsClient = true;
			}
			if (m_IsHost)
			{
				m_HostConnection = m_Connection;
			}
		}
	}
	void Networking::Connect(CSteamID id)
	{
		{
			SteamNetworkingIdentity identity;
			identity.SetSteamID(id);
			SteamNetworkingConfigValue_t options;
			options.SetPtr(k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged, (void*)ConnectionStatusChangedCallbackClient);

			m_Connection = m_Interface->ConnectP2P(identity, 0, 1, &options);

			if (m_Connection != k_HSteamNetConnection_Invalid)
			{
				m_IsClient = true;
			}
			if (m_IsHost)
			{
				m_HostConnection = m_Connection;
			}
		}
	}
	void Networking::BindScene(SharedPtr<Scene> scene)
	{
		m_Scene = scene;
		auto& registry = scene->GetRegistry();
		scene->SetComponentCreatedCallback(
			[this](entt::entity e, std::size_t id)
			{
				if (m_Scene->GetRegistry().any_of<NetworkTag>(e))
				{
					this->OnComponentCreated(e, id);  
				}
			});

		registry.on_construct<NetworkTag>().connect<&OnNetworkedEntityCreatedCallback>();
		registry.on_destroy<NetworkTag>().connect<&OnNetworkedEntityDestroyedCallback>();
	}

	void Networking::SetHandleServerAppMessageCallback(HandleAppServerMessageCallback callback)
	{
		m_HandleServerAppMessageCallback = callback;
	}

	void Networking::SetHandleClientAppMessageCallback(HandleAppMessageCallback callback)
	{
		m_HandleClientAppMessageCallback = callback;
	}

	void Networking::SetClientConnectedCallback(ClientConnectedCallback callback)
	{
		m_ClientConnectedCallback = callback;
	}

	void Networking::OnNetworkedEntityDestroyedCallback(entt::registry& registry, entt::entity e)
	{
		Get().OnNetworkedEntityDestroyed(registry, e);
	}
	
	void Networking::OnNetworkedEntityDestroyed(entt::registry& registry, entt::entity e)
	{
		if (IsHost())
		{
			AT_CORE_WARN("Entity with host handle {} destroyed.", static_cast<uint32_t>(e));

			auto* msg = new Messages::EntityDestroyedMessage(e);
			SendToAllClients(msg, sizeof(*msg));
		}
	}
	void Networking::OnNetworkedEntityCreatedCallback(entt::registry& registry, entt::entity e)
	{
		Get().OnNetworkedEntityCreated(registry, e);
	}
	void Networking::OnNetworkedEntityCreated(entt::registry& registry, entt::entity e)
	{
		if (IsHost())
		{
			AT_CORE_WARN("Entity with host handle {} created.", static_cast<uint32_t>(e));
			std::string prefabName = "";
			if(registry.any_of<PrefabTag>(e))
				prefabName = registry.get<PrefabTag>(e).Name;
			auto* msg = new Messages::EntityCreatedMessage(e, PrefabLibrary::Get().GetGUID(prefabName), registry.get<Transform>(e));
			SendToAllClients(msg, sizeof(*msg));
		}
		else if(IsClient())
		{

		}
	}
	void Networking::SendToHost( void* data, size_t size)
	{
		if (IsHost())
		{
			auto* msg = new  SteamNetworkingMessage_t();
			msg->m_conn = m_HostConnection;
			msg->m_cbSize = size;
			msg->m_pData = data;
			m_HandleServerAppMessageCallback(m_Scene, SelfClientID, msg);
		}
		else
		SendToClient(m_Connection, data, size);
	}
	void Networking::SendToAllClients(const void* msg, size_t size)
	{
		for (auto it = m_ConnectedClients.begin(); it != m_ConnectedClients.end(); it++)
		{
			SendToClient(*it, msg, size);
		}
	}
	void Networking::SendToClient(ClientID id, const void* data, size_t size)
	{
		if (id == m_HostConnection) 
		{
			auto* msg = new  SteamNetworkingMessage_t();
			msg->m_conn = m_Connection;
			msg->m_cbSize = size;
			msg->m_pData = const_cast<void*>(data);
			m_HandleClientAppMessageCallback(m_Scene,  msg);
		}
		else
		{
			auto res = m_Interface->SendMessageToConnection(id, data, size, 0, nullptr);
			if (res == k_EResultOK)
			{
				//AT_CORE_INFO("Message sent to {}", id);
			}
		}
		
	}


	bool Networking::IsHost()
	{
		return m_IsHost;
	}
	bool Networking::IsClient()
	{
		return m_IsClient;
	}
	void Networking::SetSelfClientId(HSteamNetConnection conn)
	{
		if (IsHost())
			m_HostConnection = conn;
		SelfClientID = conn;
	}
	void Networking::ReceiveMessages()
	{
		if (IsHost())
		{
			SteamNetworkingMessage_t* msg = nullptr;

			for (auto connIt = m_ConnectedClients.begin(); connIt != m_ConnectedClients.end(); ++connIt)
			{
				int num = m_Interface->ReceiveMessagesOnConnection(*connIt, &msg, 1);
				while (num > 0)
				{
					auto clientIt = m_ConnectedClients.find(msg->m_conn);
					if (clientIt == m_ConnectedClients.end())
						break;
					m_HandleServerAppMessageCallback(m_Scene, *clientIt, msg);
					msg->Release();
					num = m_Interface->ReceiveMessagesOnConnection(*connIt, &msg, 1);
				}
			}
		}

		if (IsClient())
		{
			SteamNetworkingMessage_t* msg = nullptr;
			int num = m_Interface->ReceiveMessagesOnConnection(m_Connection, &msg, 1);
			while (num > 0)
			{
				if (msg->m_cbSize)
				{
					auto type = *static_cast<const Messages::MessageType*>(msg->GetData());
					switch (type)
					{
					case Messages::EntityCreated:
					{
						if (IsHost()) break;
						auto m = *static_cast<const Messages::EntityCreatedMessage*>(msg->GetData());
						auto n = m_Scene->CreateNetworkedEntity(m.transform);
						auto h = static_cast<entt::entity>(n);
						AT_CORE_INFO("Entity created with remote handle {}, local handle {}", (uint32_t)m.e, (uint32_t)h);
						m_RemoteToLocalMap[m.e] = h;
						m_LocalToRemoteMap[h] = m.e;
						if (m.prefabId) PrefabLibrary::Get().GetPrefab(m.prefabId)->InitEntity(n);
						break;
					}
					case Messages::EntityDestroyed:
					{
						if (IsHost()) break;
						auto m = *static_cast<const Messages::EntityDestroyedMessage*>(msg->GetData());
						auto h = static_cast<entt::entity>(m_Scene->GetEntity(m_RemoteToLocalMap[m.e]));
						if (m_Scene->DestroyEntity(h))
						{
							m_RemoteToLocalMap.erase(m.e);
							m_LocalToRemoteMap.erase(h);
						}
						else
							assert(false);
						break;
					}
					case Messages::TransformUpdate:
					{
						if (IsHost()) break;
						auto m = *static_cast<const Messages::TransformUpdateMessage*>(msg->GetData());
						auto h = static_cast<entt::entity>(m_Scene->GetEntity(m_RemoteToLocalMap[m.e]));
						m_Scene->GetRegistry().get<Transform>(h) = m.transform;
						break;
					}
					default:
						m_HandleClientAppMessageCallback(m_Scene, msg);
						break;
					}
				}
				msg->Release();
				num = m_Interface->ReceiveMessagesOnConnection(m_Connection, &msg, 1);
			}
		}
	}


	entt::entity Networking::ToLocal(entt::entity e)
	{
		if (IsHost())
			return e;
		return m_RemoteToLocalMap[e];
	}
	void Networking::OnComponentCreated(entt::entity e, size_t id)
	{
		//if (IsHost())
		//{
		//	SendToAllClients(new Messages::ComponentCreatedMessage(e, id));
		//}
	}
	void Networking::ConnectionStatusChangedCallbackHost(SteamNetConnectionStatusChangedCallback_t* info)
	{
		Get().OnConnectionStatusChangedHost(info);
	}
	void Networking::OnConnectionStatusChangedHost(SteamNetConnectionStatusChangedCallback_t* status)
	{
		// Handle connection state
		switch (status->m_info.m_eState)
		{
		case k_ESteamNetworkingConnectionState_None:
			// NOTE: We will get callbacks here when we destroy connections.  You can ignore these.
			break;

		case k_ESteamNetworkingConnectionState_ClosedByPeer:
		case k_ESteamNetworkingConnectionState_ProblemDetectedLocally:
		{
			// Ignore if they were not previously connected.  (If they disconnected
			// before we accepted the connection.)
			if (status->m_eOldState == k_ESteamNetworkingConnectionState_Connected)
			{
				// Locate the client.  Note that it should have been found, because this
				// is the only codepath where we remove clients (except on shutdown),
				// and connection change callbacks are dispatched in queue order.
				auto itClient = m_ConnectedClients.find(status->m_hConn);
				//assert(itClient != m_mapClients.end());

				// Either ClosedByPeer or ProblemDetectedLocally - should be communicated to user callback
				// User callback
				//if (m_ClientDisconnectedCallback)
					//m_ClientDisconnectedCallback(itClient->second);

				m_ConnectedClients.erase(itClient);
			}
			else
			{
				//assert(info->m_eOldState == k_ESteamNetworkingConnectionState_Connecting);
			}

			// Clean up the connection.  This is important!
			// The connection is "closed" in the network sense, but
			// it has not been destroyed.  We must close it on our end, too
			// to finish up.  The reason information do not matter in this case,
			// and we cannot linger because it's already closed on the other end,
			// so we just pass 0s.
			m_Interface->CloseConnection(status->m_hConn, 0, nullptr, false);
			break;
		}

		case k_ESteamNetworkingConnectionState_Connecting:
		{
			// This must be a new connection
			// assert(m_mapClients.find(info->m_hConn) == m_mapClients.end());

			// Try to accept incoming connection
			if (m_Interface->AcceptConnection(status->m_hConn) != k_EResultOK)
			{
				m_Interface->CloseConnection(status->m_hConn, 0, nullptr, false);
				std::cout << "Couldn't accept connection (it was already closed?)" << std::endl;
				break;
			}

			// Assign the poll group
			if (!m_Interface->SetConnectionPollGroup(status->m_hConn, m_PollGroup))
			{
				m_Interface->CloseConnection(status->m_hConn, 0, nullptr, false);
				std::cout << "Failed to set poll group" << std::endl;
				break;
			}

			// Retrieve connection info
			SteamNetConnectionInfo_t connectionInfo;
			m_Interface->GetConnectionInfo(status->m_hConn, &connectionInfo);

			// Register connected client
			m_ConnectedClients.insert(status->m_hConn);
		
			//// User callback
			if (m_ClientConnectedCallback)
				m_ClientConnectedCallback(m_Scene, status->m_hConn);

			break;
		}

		case k_ESteamNetworkingConnectionState_Connected:
			// We will get a callback immediately after accepting the connection.
			// Since we are the server, we can ignore this, it's not news to us.
			AT_CORE_WARN("Client {} connected!", status->m_hConn);
			break;

		default:
			break;
		}
	}
	void Networking::ConnectionStatusChangedCallbackClient(SteamNetConnectionStatusChangedCallback_t* info)
	{
		Get().OnConnectionStatusChangedClient(info);
	}
	void Networking::OnConnectionStatusChangedClient(SteamNetConnectionStatusChangedCallback_t* info)
	{
		switch (info->m_info.m_eState)
		{
		case k_ESteamNetworkingConnectionState_None:
			// NOTE: We will get callbacks here when we destroy connections. You can ignore these.
			break;

		case k_ESteamNetworkingConnectionState_ClosedByPeer:
		case k_ESteamNetworkingConnectionState_ProblemDetectedLocally:
		{

			// Print an appropriate message
			if (info->m_eOldState == k_ESteamNetworkingConnectionState_Connecting)
			{
				// Note: we could distinguish between a timeout, a rejected connection,
				// or some other transport problem.
				std::cout << "Could not connect to remove host. " << info->m_info.m_szEndDebug << std::endl;
			}
			else if (info->m_info.m_eState == k_ESteamNetworkingConnectionState_ProblemDetectedLocally)
			{
				std::cout << "Lost connection with remote host. " << info->m_info.m_szEndDebug << std::endl;
			}
			else
			{
				// NOTE: We could check the reason code for a normal disconnection
				std::cout << "Disconnected from host. " << info->m_info.m_szEndDebug << std::endl;
			}

			// Clean up the connection.  This is important!
			// The connection is "closed" in the network sense, but
			// it has not been destroyed.  We must close it on our end, too
			// to finish up.  The reason information do not matter in this case,
			// and we cannot linger because it's already closed on the other end,
			// so we just pass 0s.
			m_Interface->CloseConnection(info->m_hConn, 0, nullptr, false);
			m_Connection = k_HSteamNetConnection_Invalid;
			break;
		}

		case k_ESteamNetworkingConnectionState_Connecting:
			// We will get this callback when we start connecting.
			// We can ignore this.
			break;

		case k_ESteamNetworkingConnectionState_Connected:
			AT_CORE_INFO("Connected");
			break;

		default:
			break;
		}
	}
}

