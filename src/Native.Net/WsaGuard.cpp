#include "WsaGuard.h"

#include <CRTDBG.H>
#include <winsock.h>

#include "Win32Exception.h"

namespace Native
{
	using namespace Windows;

	namespace Net
	{
		WsaGuard::WsaGuard()
			: _moved(false)
		{
			WSADATA wsa;

			int res = WSAStartup(MAKEWORD(2, 2), &wsa);

			if (res != ERROR_SUCCESS)
				throw Win32Exception(WSAGetLastError(), "WSAStartup(...) failed.");
		}
		WsaGuard::WsaGuard(const WsaGuard& other)
			: WsaGuard()
		{
		}
		WsaGuard::WsaGuard(WsaGuard&& other) noexcept
		{
			other._moved = true;
		}

		WsaGuard::~WsaGuard()
		{
			if (!this->_moved)
			{
				int res = WSACleanup();

				_ASSERT(res == ERROR_SUCCESS);
			}
		}
	}
}