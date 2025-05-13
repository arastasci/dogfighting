// at/networking/Networking.h
#pragma once
#include "atpch.h"
#include "at/core/Logger.h"
#include "at/core/EngineSubsystem.h"

#include "steam/isteamclient.h"
#include "steam/isteamnetworkingutils.h"
#include "steam/isteamnetworkingsockets.h"
#include "steam/isteamuser.h"
#include "steam/steam_api.h"

#include "at/ecs/CoreComponents/Transform.h"

#include <glm/gtc/quaternion.hpp>
#include <vector>

#define MAX_PLAYERS_PER_SERVER 2

namespace at
{
	/*--------------------------------------------------------------------
		Wire‑level
	--------------------------------------------------------------------*/
	enum EMessage
	{
		TransformUpdate = 1
	};


	struct ClientConnectionData_t
	{
		bool m_bActive;					// Is this slot in use? Or is it available for new connections?
		short clientID;			// What is the steamid of the player?
		uint64 m_ulTickCountLastData;	// What was the last time we got data from the player?
		HSteamNetConnection m_hConn;	// The handle for the connection to the player

		ClientConnectionData_t() {
			m_bActive = false;
			m_ulTickCountLastData = 0;
			m_hConn = 0;
		}
	};

#pragma pack(push, 1)
	struct TransformNetPacket
	{
		uint32_t msg;
		uint32_t entity;
		float    px, py, pz;
		float    rx, ry, rz, rw;
		float    sx, sy, sz;
	};
#pragma pack(pop)

	struct TransformUpdateMessage
	{
		uint32_t   entity;
		glm::vec3  position;
		glm::quat  rotation;
		glm::vec3  scale;
	};

	/*--------------------------------------------------------------------
		System
	--------------------------------------------------------------------*/
	class Networking : public EngineSubsystem<Networking>
	{
	public:
		void Init() override
		{
			SteamErrMsg errMsg = { 0 };
			if (SteamAPI_InitEx(&errMsg) != k_ESteamAPIInitResult_OK)
			{
				AT_CORE_CRITICAL("SteamAPI_InitEx failed. Steam must be running.");
				assert(false);
				return;
			}
		}

		void Shutdown() override
		{
			SteamAPI_Shutdown();
		}

		void Host()
		{
			SteamNetworkingIPAddr addr;
			addr.Clear();
			addr.m_port = m_DefaultPort;

			//SteamNetworkingConfigValue_t options;
			//options.SetPtr(k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged, (void*)Networking::OnNetConnectionStatusChanged);
			auto a = SteamNetworkingSockets();
			auto b = SteamNetworkingSockets();
			if (a == b)
			{
				std::cout << "hm" << std::endl;
			}
			m_ListenSocket = SteamNetworkingSockets()->CreateListenSocketIP(addr, 0, nullptr);

			m_isHost = true;
		}

		void Connect()
		{
			SteamNetworkingIPAddr addr;
			addr.SetIPv4(127 << 24 | 1, m_DefaultPort);
			m_Connection = SteamNetworkingSockets()->ConnectByIPAddress(addr, 0, nullptr);
			char buf[100];
			if (!SteamNetworkingSockets()->GetDetailedConnectionStatus(m_Connection, buf, 100))
			{
				Logger::GetCoreLogger()->info(buf);
			}


			m_isClient = true;
		}

		bool IsHost()   const { return m_isHost; }
		bool IsClient() const { return m_isClient; }

		/*----------------------------------------------------------------
			Send
		----------------------------------------------------------------*/
		void SendTransformUpdate(uint32_t entity, const Transform& t)
		{
			if (!SteamNetworkingSockets())
				return;
			if (m_Connection == k_HSteamNetConnection_Invalid) 
				return;

			TransformNetPacket p{};
			p.msg = TransformUpdate;
			p.entity = entity;

			p.px = t.position.x; p.py = t.position.y; p.pz = t.position.z;
			p.rx = t.rotation.x; p.ry = t.rotation.y; p.rz = t.rotation.z; p.rw = t.rotation.w;
			p.sx = t.scale.x;    p.sy = t.scale.y;    p.sz = t.scale.z;

			SteamNetworkingSockets()->SendMessageToConnection(
				m_Connection, &p, sizeof(p), k_nSteamNetworkingSend_Unreliable, nullptr);
		}

		/*----------------------------------------------------------------
			Receive
		----------------------------------------------------------------*/
		void ReceiveNetworkData()
		{
			if (!SteamNetworkingSockets())
				return;

			if (m_Connection == k_HSteamNetConnection_Invalid)
			{
				return;
			}
			SteamNetworkingMessage_t* msgs[32];
			int res = SteamNetworkingSockets()->ReceiveMessagesOnConnection(m_Connection, msgs, 32);

			for (int i = 0; i < res; ++i)
			{
				SteamNetworkingMessage_t* m = msgs[i];
				if (m->GetSize() < sizeof(uint32_t))
				{
					m->Release();
					continue;
				}

				switch ((EMessage)(*((DWORD*)m->GetData())))
				{
				case TransformUpdate:
				{
					if (m->GetSize() == sizeof(TransformNetPacket))
					{
						const auto* pkt = reinterpret_cast<const TransformNetPacket*>(m->GetData());
						TransformUpdateMessage tu;
						tu.entity = pkt->entity;
						tu.position = { pkt->px, pkt->py, pkt->pz };
						tu.rotation = { pkt->rw, pkt->rx, pkt->ry, pkt->rz }; // glm::quat(w,x,y,z)
						tu.scale = { pkt->sx, pkt->sy, pkt->sz };
						m_PendingTransformUpdates.emplace_back(tu);
					}
					break;
				}
				default: break;
				}

				m->Release();
			}
		}

		void GetTransformUpdates(std::vector<TransformUpdateMessage>& out)
		{
			out.swap(m_PendingTransformUpdates);
		}

		void RunCallback()
		{
			SteamNetworkingSockets()->RunCallbacks();
		}

	private:
		bool                     m_isHost = false;
		bool                     m_isClient = false;

		// Vector to keep track of client connections
		ClientConnectionData_t m_rgClientData[MAX_PLAYERS_PER_SERVER];

		// Vector to keep track of client connections which are pending auth
		ClientConnectionData_t m_rgPendingClientData[MAX_PLAYERS_PER_SERVER];

		HSteamNetPollGroup m_hNetPollGroup;
		std::string m_sServerName;


		void Networking::OnNetConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* pCallback);

		HSteamListenSocket       m_ListenSocket = k_HSteamListenSocket_Invalid;
		HSteamNetConnection      m_Connection = k_HSteamNetConnection_Invalid;

		const uint16             m_DefaultPort = 40002;

		std::vector<TransformUpdateMessage> m_PendingTransformUpdates;
	};
}
