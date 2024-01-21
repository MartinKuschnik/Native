#pragma once

#include <string>
#include <format>

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

			// IPv4
			IpAddress(unsigned char b1, unsigned char b2, unsigned char b3, unsigned char b4) noexcept;

			// IPv4 and IPv6 (mapped)
			IpAddress(unsigned char b1, unsigned char b2, unsigned char b3, unsigned char b4, AddressFamily af) noexcept;

			// IPv6
			IpAddress(unsigned char bytes[16]) noexcept;
			IpAddress(unsigned short words[8]) noexcept;
			IpAddress(unsigned char b1, unsigned char b2, unsigned char b3, unsigned char b4, unsigned char b5, unsigned char b6, unsigned char b7, unsigned char b8, unsigned char b9, unsigned char b10, unsigned char b11, unsigned char b12, unsigned char b13, unsigned char b14, unsigned char b15, unsigned char b16) noexcept;
			IpAddress(unsigned short w1, unsigned short w2, unsigned short w3, unsigned short w4, unsigned short w5, unsigned short w6, unsigned short w7, unsigned short w8) noexcept;

			IpAddress(in_addr address) noexcept;
			IpAddress(in6_addr address) noexcept;

			std::string string() const;
			std::wstring wstring() const;

			bool is_ipv4_mapped_to_ipv6() const;

			static IpAddress Parse(std::string_view value, Net::AddressFamily addressFamily);
			static IpAddress Parse(std::string_view value);

			static const IpAddress Any;
			static const IpAddress IPv4Any;
			static const IpAddress IPv6Any;

			bool operator ==(const IpAddress& other) const noexcept;
			bool operator !=(const IpAddress& other) const noexcept;

			operator in_addr() const;
			operator in6_addr() const;

			AddressFamily address_family() const noexcept;

			IpAddress to_v4() const;

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



template <>
struct std::formatter<Native::Net::IpAddress, char> : std::formatter<string_view, char>
{
	auto format(const Native::Net::IpAddress& ip_address, std::format_context& ctx) const
	{
		return std::formatter<string_view>::format(ip_address.string(), ctx);
	}
};

template <>
struct std::formatter<Native::Net::IpAddress, wchar_t> : std::formatter<wstring_view, wchar_t>
{
	auto format(const Native::Net::IpAddress& ip_address, std::wformat_context& ctx) const
	{
		return std::formatter<wstring_view, wchar_t>::format(ip_address.wstring(), ctx);
	}
};
