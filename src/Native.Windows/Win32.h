#pragma once

#include <string>

#include <Windows.h>

namespace Native
{
	namespace Windows
	{
		class Win32
		{
		public:
			Win32() = delete;

			static std::string GetWin32ErrorMessage(DWORD errorCode);
		};
	}
}

