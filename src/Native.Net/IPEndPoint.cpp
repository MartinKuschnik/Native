#include "IPEndPoint.h"

#include <WinSock2.h>

#include <InvalidOperationException.h>

namespace Native
{
	namespace Net
	{
		IpEndPoint::IpEndPoint() noexcept
			: Address(IpAddress::Any),
			Port(IpEndPoint::AnyPort)
		{
		}

		IpEndPoint::IpEndPoint(IpAddress ip, unsigned short port) noexcept
			: Address(ip),
			Port(port)
		{
		}

		bool IpEndPoint::operator==(const IpEndPoint& other) const noexcept
		{
			return this->Address == other.Address && this->Port == other.Port;
		}

		bool IpEndPoint::operator!=(const IpEndPoint& other) const noexcept
		{
			return this->Address != other.Address || this->Port != other.Port;
		}

		void IpEndPoint::operator = (const IpEndPoint& other) noexcept
		{
			this->Address = other.Address;
			this->Port = other.Port;
		}

		void IpEndPoint::operator = (IpEndPoint&& other) noexcept
		{
			this->Address = std::move(other.Address);
			this->Port = other.Port;
		}

		IpEndPoint::operator sockaddr_in() const
		{
			if (this->Address.address_family() != AddressFamily::InterNetwork)
				throw InvalidOperationException("IPv6 EndPoint can not be converted to and sockaddr_in (IPv4) object.");

			sockaddr_in addr{};

			addr.sin_family = AF_INET;
			addr.sin_addr = this->Address;
			addr.sin_port = htons(this->Port);

			return addr;
		}

		IpEndPoint::operator sockaddr_in6() const
		{
			if (this->Address.address_family() != AddressFamily::InterNetworkV6)
				throw InvalidOperationException("IPv4 EndPoint can not be converted to and sockaddr_in6 (IPv4) object.");

			sockaddr_in6 addr{};

			addr.sin6_family = AF_INET6;
			addr.sin6_addr = this->Address;
			addr.sin6_port = htons(this->Port);

			return addr;
		}
	}
}
