#include "UdpSocket.h"

#include <Windows.h>
#include <winsock2.h>
#include <ws2ipdef.h>

#include <CRTDBG.H>

#include "NotSupportedException.h"
#include "InvalidOperationException.h"
#include "Win32Exception.h"

namespace Native
{
	using namespace Windows;

	namespace Net
	{
		UdpSocket::UdpSocket()
			: _socket(INVALID_SOCKET),
			_boundEndPoint(std::nullopt)
		{
		}

		UdpSocket::~UdpSocket()
		{
			if (this->_socket != INVALID_SOCKET)
			{
				int result = closesocket(this->_socket);

				_ASSERT(result != SOCKET_ERROR);
			}
		}

		void UdpSocket::bind(IpEndPoint endPoint)
		{
			union {
				sockaddr_in v4;
				sockaddr_in6 v6;
			} addr{};

			int addrSize;

			if (endPoint.Address.address_family() == AddressFamily::InterNetworkV6)
			{
				// Because Any is self-constructed mechanism for IPv6 with mapped IPv4
				// the endpoint hast to be modified. 
				// Any: 0..0 FFFF 0.0.0.0	-> IPv6Any:	0..0 0000 0.0.0.0
				if (endPoint.Address == IpAddress::Any)
					addr.v6 = IpEndPoint(IpAddress::IPv6Any, endPoint.Port);
				else
					addr.v6 = endPoint;

				addrSize = sizeof(sockaddr_in6);

				this->_socket = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);

				if (endPoint.Address.is_ipv4_mapped_to_ipv6())
					this->set_ipv6_only(false);
			}
			else
			{
				addr.v4 = endPoint;
				addrSize = sizeof(sockaddr_in);

				this->_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
			}

			if (this->_socket == INVALID_SOCKET)
				throw Win32Exception(WSAGetLastError(), nameof(socket));

			int status = ::bind(this->_socket, reinterpret_cast<sockaddr*>(&addr), addrSize);

			if (status == SOCKET_ERROR)
				throw Win32Exception(WSAGetLastError(), nameof(bind));

			this->_boundEndPoint = endPoint;
		}

		void UdpSocket::bind(IpAddress ip, unsigned short port)
		{
			this->bind(IpEndPoint(ip, port));
		}

		int UdpSocket::receive_from(void* buffer, const int bufferSize, IpEndPoint* from)
		{
			if (!this->_boundEndPoint.has_value())
				throw InvalidOperationException("Socket not bound yet.");

			union {
				sockaddr_in v4;
				sockaddr_in6 v6;
			} addr{};

			int addrSize = this->address_family() == AF_INET6 ? sizeof(sockaddr_in6) : sizeof(sockaddr_in);

			int readBytes = recvfrom(this->_socket, reinterpret_cast<char*>(buffer), bufferSize, 0, reinterpret_cast<sockaddr*>(&addr), &addrSize);

			if (readBytes == SOCKET_ERROR)
				throw Win32Exception(WSAGetLastError(), nameof(recvfrom));

			if (this->address_family() == AF_INET6)
				*from = IpEndPoint(addr.v6.sin6_addr, ntohs(addr.v6.sin6_port));
			else
				*from = IpEndPoint(addr.v4.sin_addr, ntohs(addr.v4.sin_port));

			return readBytes;
		}

		int UdpSocket::send_to(void* buffer, const int bufferSize, IpEndPoint& to)
		{
			if (!this->_boundEndPoint.has_value())
				throw InvalidOperationException("Socket not bound yet.");

			const IpAddress* boundAddr = &this->_boundEndPoint.value().Address;

			const AddressFamily afTo = to.Address.address_family();
			const AddressFamily afBound = boundAddr->address_family();

			if (afTo != afBound)
				throw InvalidOperationException(boundAddr->address_family() == AddressFamily::InterNetwork ? "IpEndPoint must point to an IPv4 address." : "IpEndPoint must point to an IPv6 address.");

			if (afTo == AddressFamily::InterNetworkV6 && to.Address.is_ipv4_mapped_to_ipv6() && !boundAddr->is_ipv4_mapped_to_ipv6())
				throw InvalidOperationException("IpEndPoint must point to an pure IPv6 address. Mapped IPv4 addresses are not valid for pure IPv6 sockets.");

			union {
				sockaddr_in v4;
				sockaddr_in6 v6;
			} addr{};

			int addrSize;

			if (afTo == AddressFamily::InterNetworkV6)
			{
				addr.v6 = to;
				addrSize = sizeof(sockaddr_in6);
			}
			else
			{
				addr.v4 = to;
				addrSize = sizeof(sockaddr_in);
			}

			int writtenBytes = sendto(this->_socket, reinterpret_cast<char*>(buffer), bufferSize, 0, reinterpret_cast<sockaddr*>(&addr), addrSize);

			if (writtenBytes == SOCKET_ERROR)
				throw Win32Exception(WSAGetLastError(), nameof(sendto));

			return writtenBytes;
		}

		void UdpSocket::set_ipv6_only(const bool value)
		{
			int disable = value;

			int status = setsockopt(this->_socket, IPPROTO_IPV6, IPV6_V6ONLY, (char*)&disable, sizeof(disable));

			if (status == SOCKET_ERROR)
				throw Win32Exception(WSAGetLastError(), nameof(set_ipv6_only));

		}

		ADDRESS_FAMILY UdpSocket::address_family() const noexcept
		{
			if (!this->_boundEndPoint.has_value())
				return AF_UNSPEC;

			switch (this->_boundEndPoint.value().Address.address_family())
			{
				case AddressFamily::InterNetwork:
					return PF_INET;
				case AddressFamily::InterNetworkV6:
					return PF_INET6;
				default:
					return AF_UNKNOWN1;
			}
		}
	}
}
