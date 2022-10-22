#pragma once

#include "IpAddress.h"

namespace Native
{
	namespace Net
	{
		class IpEndPoint
		{
		public:
			IpAddress Address;
			unsigned short Port;

			IpEndPoint() noexcept;

			IpEndPoint(IpAddress ip, unsigned short port) noexcept;

			bool operator ==(const IpEndPoint& other) const noexcept;
			bool operator !=(const IpEndPoint& other) const noexcept;

			void operator = (const IpEndPoint& other) noexcept;

			static const unsigned short MinPort = 0x0000;

			static const unsigned short MaxPort = 0xFFFF;

			static const unsigned short AnyPort = IpEndPoint::MinPort;

		private:
		};

	}
}