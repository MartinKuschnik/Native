#include "CppUnitTest.h"

#include "InvalidOperationException.h"

#include "FormatException.h"
#include "IpAddress.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Native;
using namespace Native::Net;

namespace NativeNetTests
{
	TEST_CLASS(IpAddressTests)
	{
	public:

#pragma region Any & IPv6Any

		TEST_METHOD(Any_IsV4Address)
		{
			Assert::IsTrue(IpAddress::Any.address_family() == AddressFamily::InterNetwork);
		}

		TEST_METHOD(IPv6Any_IsV6Address)
		{
			Assert::IsTrue(IpAddress::IPv6Any.address_family() == AddressFamily::InterNetworkV6);
		}


		TEST_METHOD(Any_AllBytesZero)
		{
			const in_addr addr = IpAddress::Any;

			Assert::AreEqual(static_cast<UCHAR>(0), addr.S_un.S_un_b.s_b1);
			Assert::AreEqual(static_cast<UCHAR>(0), addr.S_un.S_un_b.s_b2);
			Assert::AreEqual(static_cast<UCHAR>(0), addr.S_un.S_un_b.s_b3);
			Assert::AreEqual(static_cast<UCHAR>(0), addr.S_un.S_un_b.s_b4);
		}

		TEST_METHOD(IPv6Any_AllBytesZero)
		{
			const in6_addr addr = IpAddress::IPv6Any;

			for (size_t i = 0; i < 15; i++)
				Assert::AreEqual(static_cast<UCHAR>(0), addr.u.Byte[i]);
		}

#pragma endregion

#pragma region Operators

		TEST_METHOD(TypeCastOerator_in6_addr_For_IPv4_ThrowsInvalidOperationException)
		{
			try
			{
				const in6_addr v6addr = IpAddress::Any;

				Assert::Fail(L"InvalidOperationException did not throw!");
			}
			catch (const InvalidOperationException&)
			{
				// exception successfully thrown
			}
		}

		TEST_METHOD(TypeCastOerator_in_addr_For_IPv6_ThrowsInvalidOperationException)
		{
			try
			{
				const in_addr v4addr = IpAddress::IPv6Any;

				Assert::Fail(L"InvalidOperationException did not throw!");
			}
			catch (const InvalidOperationException&)
			{
				// exception successfully thrown
			}
		}
#pragma endregion

#pragma region Parse

		TEST_METHOD(Parse_IpV4Address)
		{
			IpAddress ipv4 = IpAddress::Parse("192.168.178.1");

			Assert::IsTrue(ipv4.address_family() == AddressFamily::InterNetwork);
		}

		TEST_METHOD(Parse_ThrowsFormatExceptionForInvalidIpv4)
		{
			try
			{
				IpAddress::Parse("192.168.178");

				Assert::Fail(L"IpAddress::Parse(..) did not throw.");
			}
			catch (const FormatException&)
			{
				// exception successfully thrown
			}
		}

		TEST_METHOD(Parse_IpV6Address)
		{
			IpAddress ipv6 = IpAddress::Parse("2001:0db8:85a3:0000:0000:8a2e:0370:7334");

			Assert::IsTrue(ipv6.address_family() == AddressFamily::InterNetworkV6);
		}

		TEST_METHOD(Parse_ThrowsFormatExceptionForInvalidIpv6Address)
		{
			try
			{
				IpAddress::Parse("2001:0db8:85a3:0000:0000:8a2e:0370:");

				Assert::Fail(L"IpAddress::Parse(..) did not throw.");
			}
			catch (const FormatException&)
			{
				// exception successfully thrown
			}
		}

		TEST_METHOD(Parse_IPv4MappedToIPv6Address)
		{
			IpAddress ipv6 = IpAddress::Parse("::ffff:192.0.2.128");

			Assert::IsTrue(ipv6.address_family() == AddressFamily::InterNetworkV6);
		}

		TEST_METHOD(Parse_ThrowsFormatExceptionForInvalidIPv4MappedToIPv6Address)
		{
			try
			{
				IpAddress::Parse("::ffff:256.0.2.128");

				Assert::Fail(L"IpAddress::Parse(..) did not throw.");
			}
			catch (const FormatException&)
			{
				// exception successfully thrown
			}
		}

#pragma endregion

#pragma region to_string

		TEST_METHOD(to_string_IpV4)
		{
			in_addr addr{};
			addr.S_un.S_un_b.s_b1 = 100;
			addr.S_un.S_un_b.s_b2 = 5;
			addr.S_un.S_un_b.s_b3 = 200;
			addr.S_un.S_un_b.s_b4 = 12;

			IpAddress ipv4Address(addr);

			std::string ipv4_addr_as_string = ipv4Address.to_string();

			Assert::AreEqual("100.5.200.12", ipv4_addr_as_string.c_str());
		}

		TEST_METHOD(to_string_IpV6)
		{
			in6_addr addr{};

			addr.u = { 0x20, 0x01, 0x0d, 0xb8, 0x85, 0xa3, 0x00, 0x00,  0x00, 0x00 , 0x8a, 0x2e, 0x03, 0x70, 0x73, 0x34 };

			IpAddress ipv6Address(addr);

			std::string ipv6_addr_as_string = ipv6Address.to_string();

			Assert::AreEqual("2001:db8:85a3::8a2e:370:7334", ipv6_addr_as_string.c_str());
		}

		TEST_METHOD(to_string_IPv4MappedToIPv6)
		{
			in6_addr addr{};

			// IPv4 mapped to IPv6 Addresses consist of an 80-bit prefix of zeros,
			// least-significant 32 bits contain the IPv4 address.
			// the next 16 bits are ones, and the remaining, 
			addr.u = {
				0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
				0xFF,0xFF,
				192, 0, 2, 128 };

			IpAddress ipv6Address(addr);

			std::string ipv6_addr_as_string = ipv6Address.to_string();

			Assert::AreEqual("::ffff:192.0.2.128", ipv6_addr_as_string.c_str());
		}

#pragma endregion

#pragma region is_ipv4_mapped_to_ipv6

		TEST_METHOD(is_ipv4_mapped_to_ipv6_IPv4)
		{
			in_addr addr{};
			addr.S_un.S_un_b.s_b1 = 100;
			addr.S_un.S_un_b.s_b2 = 5;
			addr.S_un.S_un_b.s_b3 = 200;
			addr.S_un.S_un_b.s_b4 = 12;

			IpAddress ipv4Address(addr);

			Assert::IsFalse(ipv4Address.is_ipv4_mapped_to_ipv6());
		}

		TEST_METHOD(is_ipv4_mapped_to_ipv6_IPv6)
		{
			in6_addr addr{};

			addr.u = { 0x20, 0x01, 0x0d, 0xb8, 0x85, 0xa3, 0x00, 0x00,  0x00, 0x00 , 0x8a, 0x2e, 0x03, 0x70, 0x73, 0x34 };

			IpAddress ipv6Address(addr);

			Assert::IsFalse(ipv6Address.is_ipv4_mapped_to_ipv6());
		}

		TEST_METHOD(is_ipv4_mapped_to_ipv6_IPv4MappedToIPv6)
		{
			in6_addr addr{};

			// IPv4 mapped to IPv6 Addresses consist of an 80-bit prefix of zeros,
			// least-significant 32 bits contain the IPv4 address.
			// the next 16 bits are ones, and the remaining, 
			addr.u = {
				0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
				0xFF,0xFF,
				192, 0, 2, 128 };

			IpAddress ipv6Address(addr);

			Assert::IsTrue(ipv6Address.is_ipv4_mapped_to_ipv6());
		}

#pragma endregion

	};
}
