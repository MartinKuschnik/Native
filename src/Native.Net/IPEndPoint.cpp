#include "IPEndPoint.h"

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
	}
}
