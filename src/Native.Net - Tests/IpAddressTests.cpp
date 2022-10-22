#include "CppUnitTest.h"

#include "InvalidOperationException.h"

#include "IpAddress.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Native;
using namespace Native::Net;

namespace NativeNetTests
{
	TEST_CLASS(IpAddressTests)
	{
	public:
		
		TEST_METHOD(Any_IsV4Address)
		{
			Assert::IsTrue(IpAddress::Any.address_family() == AddressFamily::InterNetwork);
		}

		TEST_METHOD(IPv6Any_IsV6Address)
		{
			Assert::IsTrue( IpAddress::IPv6Any.address_family() ==AddressFamily::InterNetworkV6);
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
	};
}
