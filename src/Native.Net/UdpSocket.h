#pragma once

#include <optional>

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

			void bind(IpEndPoint endPoint);
			void bind(IpAddress ip, unsigned short port);

			int receive_from(void* buffer, const int bufferSize, IpEndPoint* from);
			int send_to(void* buffer, const int bufferSize, IpEndPoint& to);

		private:

			// ensures WSAStartup und WSACleanup are called
			const WsaGuard _wsaGuard;

			std::optional<IpEndPoint> _boundEndPoint;

			INT_PTR _socket;

			void set_ipv6_only(const bool value);

			ADDRESS_FAMILY address_family() const noexcept;
		};
	}
}