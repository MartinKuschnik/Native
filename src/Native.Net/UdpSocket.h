#pragma once

#include <Windows.h>

#include "IPEndPoint.h"
#include "WsaGuard.h"

namespace Native
{
	namespace Net
	{
		class UdpSocket
		{
		public:
			UdpSocket();
			~UdpSocket();

			void bind(unsigned short port);

			int receive_from(void* buffer, const int bufferSize, IpEndPoint* from);
			int send_to(void* buffer, const int bufferSize, IpEndPoint& to);

		private:

			// ensures WSAStartup und WSACleanup are called
			const WsaGuard _wsaGuard;

			INT_PTR _socket;
		};
	}
}