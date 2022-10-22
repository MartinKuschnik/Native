#include "UdpSocket.h"

#include <Windows.h>
#include <winsock2.h>
#include <ws2ipdef.h>

#include <CRTDBG.H>

#include "NotSupportedException.h"
#include "Win32Exception.h"

namespace Native
{
	using namespace Windows;

	namespace Net
	{
		UdpSocket::UdpSocket()
		{
			this->_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

			if (this->_socket == INVALID_SOCKET)
				throw Win32Exception(WSAGetLastError(), nameof(socket));
		}

		UdpSocket::~UdpSocket()
		{
			int result = closesocket(this->_socket);

			_ASSERT(result != SOCKET_ERROR);
		}

		void UdpSocket::bind(unsigned short port)
		{
			sockaddr_in addr = {};

			addr.sin_family = AF_INET;
			addr.sin_port = htons(port);

			int status = ::bind(this->_socket, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));

			if (status == SOCKET_ERROR)
				throw Win32Exception(WSAGetLastError(), nameof(bind));
		}

		int UdpSocket::receive_from(void* buffer, const int bufferSize, IpEndPoint* from)
		{
			sockaddr_in fromAddress;
			int size = sizeof(fromAddress);

			int readBytes = recvfrom(this->_socket, reinterpret_cast<char*>(buffer), bufferSize, 0, reinterpret_cast<sockaddr*>(&fromAddress), &size);

			if (readBytes == SOCKET_ERROR)
				throw Win32Exception(WSAGetLastError(), nameof(recvfrom));

			*from = IpEndPoint(fromAddress.sin_addr, ntohs(fromAddress.sin_port));

			return readBytes;
		}

		int UdpSocket::send_to(void* buffer, const int bufferSize, IpEndPoint& to)
		{
			if (to.Address.address_family() == AddressFamily::InterNetworkV6)
				throw NotSupportedException("IPv6 is not supported.");

			sockaddr_in toAddress{};

			toAddress.sin_family = AF_INET;
			toAddress.sin_port = htons(to.Port);
			toAddress.sin_addr = to.Address;

			int writtenBytes = sendto(this->_socket, reinterpret_cast<char*>(buffer), bufferSize, 0, reinterpret_cast<sockaddr*>(&toAddress), sizeof(toAddress));

			if (writtenBytes == SOCKET_ERROR)
				throw Win32Exception(WSAGetLastError(), nameof(sendto));

			return writtenBytes;
		}

		//void UdpSocket::set_ipv6_only(const bool value)
		//{
		//	int disable = value;

		//	int status = setsockopt(this->_socket, IPPROTO_IPV6, IPV6_V6ONLY, (char*)&disable, sizeof(disable));

		//	if (status == SOCKET_ERROR)
		//		throw Win32Exception(WSAGetLastError(), "setsockopt(...) failed.");

		//}
	}
}
