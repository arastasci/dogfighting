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
		/*if (IsHost())
		{
			UpdateDirtyComponents();
		}*/
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
		m_ListenSocket = m_Interface->CreateListenSocketIP(addr, 1, &options);

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

	void Networking::OnNetworkedEntityDestroyedCallback(entt::registry& registry, entt::entity e)
	{
		Get().OnNetworkedEntityDestroyed(registry, e);
	}
	
	void Networking::OnNetworkedEntityDestroyed(entt::registry& registry, entt::entity e)
	{
		if (IsHost())
		{
			SendToAllClients(new Messages::EntityDestroyedMessage(e));
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
			SendToAllClients(new Messages::EntityCreatedMessage(e));
		}
		else if(IsClient())
		{

		}
	}
	void Networking::SendToHost( void* data)
	{
		if (IsHost())
		{
			auto* msg = new  SteamNetworkingMessage_t();
			msg->m_conn = m_Connection;
			msg->m_cbSize = sizeof(data);
			msg->m_pData = data;
			m_HandleServerAppMessageCallback(m_Scene, *m_ConnectedClients.begin(), msg);
		}
		else
		SendToClient(m_Connection, data);
	}
	void Networking::SendToAllClients(const void* msg)
	{
		for (auto it = ++m_ConnectedClients.begin(); it != m_ConnectedClients.end(); it++)
		{
			SendToClient(*it, msg);
		}
	}
	void Networking::SendToClient(ClientID id, const void* data)
	{
		auto res = m_Interface->SendMessageToConnection(id, data, sizeof(data), 0, nullptr);
		return;
	}


	bool Networking::IsHost()
	{
		return m_IsHost;
	}
	bool Networking::IsClient()
	{
		return m_IsClient;
	}
	void Networking::ReceiveMessages()
	{

		if (IsHost())
		{
			SteamNetworkingMessage_t* incomingMessage = nullptr;
			
			int messageCount = m_Interface->ReceiveMessagesOnPollGroup(m_PollGroup, &incomingMessage, 1);
			if (messageCount <= 0)
			{
				return;
			}
			auto itClient = m_ConnectedClients.find(incomingMessage->m_conn);
			if (itClient == m_ConnectedClients.end())
			{
				std::cout << "ERROR: Received data from unregistered client\n";
				return;
			}
			m_HandleServerAppMessageCallback(m_Scene, *itClient, incomingMessage);
			
		}
		else if(IsClient())
		{
			SteamNetworkingMessage_t* incomingMessage = nullptr;
			int messageCount = m_Interface->ReceiveMessagesOnConnection(m_Connection, &incomingMessage, 1);
			if (messageCount == 0)
			{
				return;
			}

			if (incomingMessage->m_cbSize)
			{
				AT_CORE_INFO("We got a message");
				Messages::MessageType e = *(static_cast<const Messages::MessageType*>((incomingMessage->GetData())));

				switch (e)
				{
				case Messages::EntityCreated:
				{
					auto msg = *static_cast<const Messages::EntityCreatedMessage*>((incomingMessage->GetData()));
					AT_CORE_INFO("Entity created with handle {}", static_cast<uint32_t>(msg.e));
					auto handle = static_cast<entt::entity>(m_Scene->CreateNetworkedEntity());
					m_RemoteToLocalMap[msg.e] = handle;
					m_LocalToRemoteMap[handle] = msg.e;
					break;
				}
				case Messages::EntityDestroyed:
				{
					auto msg = *static_cast<const Messages::EntityDestroyedMessage*>((incomingMessage->GetData()));
					AT_CORE_INFO("Entity destroyed with handle {}", static_cast<uint32_t>(msg.e));
					auto entity = static_cast<entt::entity>(m_Scene->GetEntity(m_RemoteToLocalMap[msg.e]));
					if (m_Scene->DestroyEntity(entity))
					{
						m_RemoteToLocalMap.erase(msg.e);
						m_LocalToRemoteMap.erase(entity);
					}
					else
					{
						assert(false);
					}
					break;
				}
				case Messages::TransformUpdate:
				{
					auto msg = *static_cast<const Messages::TransformUpdateMessage*>((incomingMessage->GetData()));
					auto entity = static_cast<entt::entity>(m_Scene->GetEntity(m_RemoteToLocalMap[msg.e]));
					auto& t = m_Scene->GetRegistry().get<Transform>(entity);
					t = msg.transform;
					break;
				}
				default:
				{
					m_HandleClientAppMessageCallback(m_Scene, incomingMessage);
					break;
				}
				}

			}

		}
	}
	entt::entity Networking::ToLocal(entt::entity e)
	{
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
			//if (m_ClientConnectedCallback)
			//	m_ClientConnectedCallback(client);

			break;
		}

		case k_ESteamNetworkingConnectionState_Connected:
			// We will get a callback immediately after accepting the connection.
			// Since we are the server, we can ignore this, it's not news to us.
			Logger::GetCoreLogger()->warn("Connected!");
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

