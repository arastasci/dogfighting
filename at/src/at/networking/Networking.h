#pragma once
#include "atpch.h"
#include "at/core/Logger.h"
#include "at/core/EngineSubsystem.h"
#include "steam/isteamclient.h"
#include "steam/isteamnetworkingutils.h"
#include "steam/isteamnetworkingsockets.h"
#include "steam/isteamuser.h"
#include "steam/steam_api.h"
namespace at
{
	class Networking : public EngineSubsystem<Networking>
	{
	public:
		virtual void Init() override
		{
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
		}

		virtual void Shutdown() override
		{

		}

		void Host()
		{

		}

		void Connect()
		{

		}
	private:
		CSteamID m_SteamID;
	};
}