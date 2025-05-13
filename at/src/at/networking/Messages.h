#pragma once
#include "atpch.h"
#include <minwindef.h>
#include <steam/steamtypes.h>

namespace at
{

	// Msg from client to server when sending state update
	struct MsgClientSendLocalUpdate_t
	{
		MsgClientSendLocalUpdate_t() : m_dwMessageType((k_EMsgClientSendLocalUpdate)) {}
		DWORD GetMessageType() { return m_dwMessageType; }

		void SetShipPosition(uint32 uPos) { m_uShipPosition = (uPos); }
		ClientSpaceWarUpdateData_t* AccessUpdateData() { return &m_ClientUpdateData; }

	private:
		const DWORD m_dwMessageType;

		uint32 m_uShipPosition;
		ClientSpaceWarUpdateData_t m_ClientUpdateData;
	};

}