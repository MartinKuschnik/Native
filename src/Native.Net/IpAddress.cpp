#include "IpAddress.h"

#include <ws2tcpip.h>

#include <format>

#include "InvalidOperationException.h"
#include "FormatException.h"
#include "Win32Exception.h"

namespace Native
{
	using namespace Native::Windows;

	namespace Net
	{
		const IpAddress IpAddress::Any = IpAddress(0, 0, 0, 0, AddressFamily::InterNetworkV6);
		const IpAddress IpAddress::IPv4Any = IpAddress(0, 0, 0, 0, AddressFamily::InterNetwork);
		const IpAddress IpAddress::IPv6Any = IpAddress( 0, 0, 0, 0, 0, 0, 0, 0  );

		// IPv4
		IpAddress::IpAddress(unsigned char b1, unsigned char b2, unsigned char b3, unsigned char b4) noexcept
			: _address({ .v4 = { b1, b2, b3, b4 } }),
			_addressFamily(AddressFamily::InterNetwork)
		{
		}

		// IPv4 and IPv6 (mapped)
		IpAddress::IpAddress(unsigned char b1, unsigned char b2, unsigned char b3, unsigned char b4, AddressFamily af) noexcept
			: _addressFamily(af)
		{
			if (af == AddressFamily::InterNetwork)
				this->_address.v4 = in_addr{ b1, b2, b3, b4 };
			else
				this->_address.v6 = in6_addr{ .u = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xFF, 0xFF, b1, b2, b3, b4} };
		}

		// IPv6
		IpAddress::IpAddress(unsigned char bytes[16]) noexcept
			: _address({ .v6 = in6_addr {.u = {.Byte = *bytes} } }),
			_addressFamily(AddressFamily::InterNetworkV6)
		{

		}

		// IPv6
		IpAddress::IpAddress(unsigned char b1, unsigned char b2, unsigned char b3, unsigned char b4, unsigned char b5, unsigned char b6, unsigned char b7, unsigned char b8, unsigned char b9, unsigned char b10, unsigned char b11, unsigned char b12, unsigned char b13, unsigned char b14, unsigned char b15, unsigned char b16) noexcept
			: _address({ .v6 = in6_addr {.u = {.Byte = {b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, b16}} } }),
			_addressFamily(AddressFamily::InterNetworkV6)
		{
		}

		// IPv6
		IpAddress::IpAddress(unsigned short words[8]) noexcept
			: _address({ .v6 = in6_addr {.u = {.Word = *words} } }),
			_addressFamily(AddressFamily::InterNetworkV6)
		{

		}

		// IPv6
		IpAddress::IpAddress(unsigned short w1, unsigned short w2, unsigned short w3, unsigned short w4, unsigned short w5, unsigned short w6, unsigned short w7, unsigned short w8) noexcept
			: _address({ .v6 = in6_addr {.u = {.Word = {w1, w2, w3, w4, w5 , w6, w7, w8}} } }),
			_addressFamily(AddressFamily::InterNetworkV6)
		{
		}

		IpAddress::IpAddress(in_addr address) noexcept
			: _address({ .v4 = address }),
			_addressFamily(AddressFamily::InterNetwork)
		{

		}

		IpAddress::IpAddress(in6_addr address) noexcept
			: _address({ .v6 = address }),
			_addressFamily(AddressFamily::InterNetworkV6)
		{

		}

		std::string IpAddress::string() const
		{
			std::string addressString(this->_addressFamily == AddressFamily::InterNetworkV6 ? 46 : 16, '\0');

			const ADDRESS_FAMILY af = this->_addressFamily == AddressFamily::InterNetworkV6 ? AF_INET6 : AF_INET;

			auto res = inet_ntop(af, &this->_address, addressString.data(), addressString.capacity());

			if (res == nullptr)
				throw Win32Exception(WSAGetLastError(), nameof(inet_ntop));

			addressString.resize(addressString.find('\0'));

			return addressString;
		}

		std::wstring IpAddress::wstring() const
		{
			std::wstring addressString(this->_addressFamily == AddressFamily::InterNetworkV6 ? 46 : 16, '\0');

			const ADDRESS_FAMILY af = this->_addressFamily == AddressFamily::InterNetworkV6 ? AF_INET6 : AF_INET;

			auto res = InetNtopW(af, &this->_address, addressString.data(), addressString.capacity());

			if (res == nullptr)
				throw Win32Exception(WSAGetLastError(), nameof(inet_ntop));

			addressString.resize(addressString.find(L'\0'));

			return addressString;
		}

		bool IpAddress::is_ipv4_mapped_to_ipv6() const
		{
			// IPv4 mapped to IPv6 Addresses consist of an 80-bit prefix of zeros,
			// the next 16 bits are ones, and the remaining, 
			// least-significant 32 bits contain the IPv4 address.

			return this->_addressFamily == AddressFamily::InterNetworkV6
				&& this->_address.data.hi == 0x00
				&& this->_address.v6.u.Byte[8] == 0x00
				&& this->_address.v6.u.Byte[9] == 0x00
				&& this->_address.v6.u.Byte[10] == 0xFF
				&& this->_address.v6.u.Byte[11] == 0xFF;
		}

		AddressFamily IpAddress::address_family() const noexcept
		{
			return this->_addressFamily;
		}

		IpAddress IpAddress::to_v4() const
		{
			if (this->_addressFamily == AddressFamily::InterNetwork)
				return *this;

			if (!this->is_ipv4_mapped_to_ipv6())
				throw InvalidOperationException(std::format("The address {0} is no IPv6 mapped IPv4 address.", this->string()));

			return IpAddress(this->_address.v6.u.Byte[12], this->_address.v6.u.Byte[13], this->_address.v6.u.Byte[14], this->_address.v6.u.Byte[15]);
		}

		IpAddress IpAddress::Parse(std::string_view value)
		{
			if (value.find(':') != std::string_view::npos)
				return IpAddress::ParseV6(value);
			else
				return IpAddress::ParseV4(value);
		}

		bool IpAddress::operator==(const IpAddress& other) const noexcept
		{
			return this->_addressFamily == other._addressFamily
				&& this->_address.data.hi == other._address.data.hi
				&& this->_address.data.low == other._address.data.low;
		}

		bool IpAddress::operator!=(const IpAddress& other) const noexcept
		{
			return this->_addressFamily != other._addressFamily
				|| this->_address.data.hi != other._address.data.hi
				|| this->_address.data.low != other._address.data.low;
		}

		IpAddress::operator in_addr() const
		{
			if (this->_addressFamily == AddressFamily::InterNetworkV6)
				throw InvalidOperationException(std::format("IPv6 address can not be converted into {0}.", nameof(in_addr)));

			return this->_address.v4;
		}

		IpAddress::operator in6_addr() const
		{
			if (this->_addressFamily == AddressFamily::InterNetwork)
				throw InvalidOperationException(std::format("IPv4 address can not be converted into {0}.", nameof(in6_addr)));

			return this->_address.v6;
		}

		IpAddress IpAddress::ParseV4(std::string_view value)
		{
			in_addr address;

			int status = inet_pton(AF_INET, value.data(), &address);

			switch (status)
			{
				case 1:
					return IpAddress(address);
				case 0:
					throw FormatException(std::format("\"{0}\" is no valid IP address.", value));
				case -1:
				default:
					throw Win32Exception(WSAGetLastError(), nameof(inet_pton));
			}
		}

		IpAddress IpAddress::ParseV6(std::string_view value)
		{
			in6_addr address;

			int status = inet_pton(AF_INET6, value.data(), &address);

			switch (status)
			{
				case 1:
					return IpAddress(address);
				case 0:
					throw FormatException(std::format("\"{0}\" is no valid IP v6 address.", value));
				case -1:
				default:
					throw Win32Exception(WSAGetLastError(), nameof(inet_pton));
			}
		}

		IpAddress IpAddress::Parse(std::string_view value, Net::AddressFamily addressFamily)
		{
			if (addressFamily == AddressFamily::InterNetworkV6)
				return IpAddress::ParseV6(value);
			else
				return IpAddress::ParseV4(value);
		}
	}
}
