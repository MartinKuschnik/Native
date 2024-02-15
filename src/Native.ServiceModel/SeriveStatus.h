#pragma once

#include <Windows.h>

namespace Native
{
	namespace ServiceModel
	{
		enum class SeriveStatus : DWORD
		{
			Stopped = SERVICE_STOPPED,
			Start_Pending = SERVICE_START_PENDING,
			Stop_Pending = SERVICE_STOP_PENDING,
			Running = SERVICE_RUNNING,
			Continue_Pending = SERVICE_CONTINUE_PENDING,
			Pause_Pending = SERVICE_PAUSE_PENDING,
			Paused = SERVICE_PAUSED
		};
	}
}