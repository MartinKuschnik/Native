#pragma once

#include <string>

#include <Windows.h>
#include <ws2def.h>
#include <ws2ipdef.h>

#include "AddressFamily.h"

namespace Native
{
	namespace Net
	{
		class IpAddress
		{
		public:


			IpAddress() = delete;

			IpAddress(in_addr address) noexcept;
			IpAddress(in6_addr address) noexcept;

			std::string to_string() const;

			bool is_ipv4_mapped_to_ipv6() const;

			static IpAddress Parse(std::string_view value, Net::AddressFamily addressFamily);
			static IpAddress Parse(std::string_view value);

			static const IpAddress Any;
			static const IpAddress IPv6Any;

			bool operator ==(const IpAddress& other) const noexcept;
			bool operator !=(const IpAddress& other) const noexcept;

			operator in_addr() const;
			operator in6_addr() const;

			AddressFamily address_family() const noexcept;

		private:

			AddressFamily _addressFamily;
			union
			{
				in_addr v4;
				in6_addr v6;

				// used for comparison
				struct {
					unsigned long long hi;
					unsigned long long low;
				} data;

			} _address;


			static IpAddress ParseV4(std::string_view value);
			static IpAddress ParseV6(std::string_view value);
		};

	}
}
