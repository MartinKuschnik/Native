#pragma once

#include <Windows.h>

namespace Native
{
	namespace ServiceModel
	{
		enum class SeriveType : DWORD
		{
			Kernel_Driver = SERVICE_KERNEL_DRIVER,
			File_System_Driver = SERVICE_FILE_SYSTEM_DRIVER,
			Adapter = SERVICE_ADAPTER,
			Recognizer_Sriver = SERVICE_RECOGNIZER_DRIVER,
			Driver = SERVICE_DRIVER,
			Win32_Own_Process = SERVICE_WIN32_OWN_PROCESS,
			Win32_Share_Process = SERVICE_WIN32_SHARE_PROCESS,
			Win32 = SERVICE_WIN32,
			User_Service = SERVICE_USER_SERVICE,
			Userservice_Instanc = SERVICE_USERSERVICE_INSTANCE,
			User_Share_Process = SERVICE_USER_SHARE_PROCESS,
			User_Own_Process = SERVICE_USER_OWN_PROCESS,
			Interactive_Process = SERVICE_INTERACTIVE_PROCESS,
			Pkg_Service = SERVICE_PKG_SERVICE,
			Type_All = SERVICE_TYPE_ALL
		};
	}
}