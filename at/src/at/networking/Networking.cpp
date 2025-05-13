#include "Networking.h"

namespace at
{
	void Networking::OnNetConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* pCallback)
	{
		/// Connection handle
		HSteamNetConnection hConn = pCallback->m_hConn;

		/// Full connection info
		SteamNetConnectionInfo_t info = pCallback->m_info;

		/// Previous state.  (Current state is in m_info.m_eState)
		ESteamNetworkingConnectionState eOldState = pCallback->m_eOldState;

		// Parse information to know what was changed

		// Check if a client has connected
		if (info.m_hListenSocket &&
			eOldState == k_ESteamNetworkingConnectionState_None &&
			info.m_eState == k_ESteamNetworkingConnectionState_Connecting)
		{
			// Connection from a new client
			// Search for an available slot
			for (uint32 i = 0; i < MAX_PLAYERS_PER_SERVER; ++i)
			{
				if (!m_rgClientData[i].m_bActive && !m_rgPendingClientData[i].m_hConn)
				{

					// Found one.  "Accept" the connection.
					EResult res = SteamNetworkingSockets()->AcceptConnection(hConn);
					if (res != k_EResultOK)
					{
						char msg[256];
						sprintf(msg, "AcceptConnection returned %d", res);
						OutputDebugString(msg);
						SteamNetworkingSockets()->CloseConnection(hConn, k_ESteamNetConnectionEnd_AppException_Generic, "Failed to accept connection", false);
						return;
					}

					m_rgPendingClientData[i].m_hConn = hConn;

					// add the user to the poll group
					SteamNetworkingSockets()->SetConnectionPollGroup(hConn, m_hNetPollGroup);

					//SteamNetworkingSockets()->SendMessageToConnection(hConn, &msg, sizeof(MsgServerSendInfo_t), k_nSteamNetworkingSend_Reliable, nullptr);


					return;
				}
			}

			// No empty slots.  Server full!
			AT_CORE_INFO("Rejecting connection; server full");
			SteamNetworkingSockets()->CloseConnection(hConn, k_ESteamNetConnectionEnd_AppException_Generic, "Server full!", false);
		}
		//// Check if a client has disconnected
		//else if ((eOldState == k_ESteamNetworkingConnectionState_Connecting || eOldState == k_ESteamNetworkingConnectionState_Connected) &&
		//	info.m_eState == k_ESteamNetworkingConnectionState_ClosedByPeer)
		//{
		//	// Handle disconnecting a client
		//	for (uint32 i = 0; i < MAX_PLAYERS_PER_SERVER; ++i)
		//	{
		//		// If there is no ship, skip
		//		if (!m_rgClientData[i].m_bActive)
		//			continue;

		//		if (m_rgClientData[i].clientID == info.m_identityRemote.GetSteamID())//pCallback->m_steamIDRemote)
		//		{
		//			OutputDebugString("Disconnected dropped user\n");
		//			RemovePlayerFromServer(i, k_EDRClientDisconnect);
		//			break;
		//		}
		//	}
		//}
	}
}