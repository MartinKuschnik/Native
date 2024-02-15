#pragma once

#include <Windows.h>

namespace Native
{
	namespace ServiceModel
	{
		enum class SeriveControlsAccepted : DWORD
		{
			Accept_Stop = SERVICE_ACCEPT_STOP,
			Accept_Pause_continue = SERVICE_ACCEPT_PAUSE_CONTINUE,
			Accept_Shutdown = SERVICE_ACCEPT_SHUTDOWN,
			Accept_Paramchange = SERVICE_ACCEPT_PARAMCHANGE,
			Accept_Netbindchange = SERVICE_ACCEPT_NETBINDCHANGE,
			Accept_Hardwareprofilechange = SERVICE_ACCEPT_HARDWAREPROFILECHANGE,
			Accept_Powerevent = SERVICE_ACCEPT_POWEREVENT,
			Accept_Sessionchange = SERVICE_ACCEPT_SESSIONCHANGE,
			Accept_Preshutdown = SERVICE_ACCEPT_PRESHUTDOWN,
			Accept_Timechange = SERVICE_ACCEPT_TIMECHANGE,
			Accept_Triggerevent = SERVICE_ACCEPT_TRIGGEREVENT,
			Accept_User_Logoff = SERVICE_ACCEPT_USER_LOGOFF,
			Accept_Lowresources = SERVICE_ACCEPT_LOWRESOURCES,
			Accept_Systemlowresources = SERVICE_ACCEPT_SYSTEMLOWRESOURCES
		};

		SeriveControlsAccepted operator|(SeriveControlsAccepted lhs, SeriveControlsAccepted rhs)
		{
			return static_cast<SeriveControlsAccepted>(static_cast<DWORD>(lhs) | static_cast<DWORD>(rhs));
		}
	}
}