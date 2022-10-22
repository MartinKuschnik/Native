#include "IpAddress.h"

#include <ws2tcpip.h>

#include <fmt/format.h>

#include "InvalidOperationException.h"
#include "FormatException.h"
#include "Win32Exception.h"

namespace Native
{
	using namespace Native::Windows;

	namespace Net
	{
		const IpAddress IpAddress::Any = IpAddress(in_addr{});
		const IpAddress IpAddress::IPv6Any = IpAddress(in6_addr{});

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

		std::string IpAddress::to_string() const
		{
			std::string addressString(this->_addressFamily == AddressFamily::InterNetworkV6 ? 46 : 16, '\0');

			const ADDRESS_FAMILY af = this->_addressFamily == AddressFamily::InterNetworkV6 ? AF_INET6 : AF_INET;

			auto res = inet_ntop(af, &this->_address, addressString.data(), addressString.capacity());

			if (res == nullptr)
				throw Win32Exception(WSAGetLastError(), nameof(inet_ntop));

			addressString.resize(addressString.find('\0'));

			return addressString;
		}

		AddressFamily IpAddress::address_family() const noexcept
		{
			return this->_addressFamily;
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
				throw InvalidOperationException(fmt::format("IPv6 address can not be converted into {0}.", nameof(in_addr)));

			return this->_address.v4;
		}

		IpAddress::operator in6_addr() const
		{
			if (this->_addressFamily == AddressFamily::InterNetwork)
				throw InvalidOperationException(fmt::format("IPv4 address can not be converted into {0}.", nameof(in6_addr)));
			
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
					throw FormatException(fmt::format("\"{0}\" is no valid IP address.", value));
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
					throw FormatException(fmt::format("\"{0}\" is no valid IP v6 address.", value));
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
