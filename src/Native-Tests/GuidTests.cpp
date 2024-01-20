#include "CppUnitTest.h"

#include "Guid.h"

using namespace Native;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace NativeTests
{
	TEST_CLASS(GuidTests)
	{

	public:
		TEST_METHOD(Parse_ThrowsExceptionForInvalidFormat)
		{
			Assert::ExpectException<std::exception>([]() { Guid::Parse(L"This is not a Guid!!!"); });
		}

		TEST_METHOD(GuidEmpty_wstring_AllZero)
		{
			Assert::AreEqual(std::wstring(L"00000000-0000-0000-0000-000000000000"), static_cast<std::wstring>(Guid::Empty));
		}

		TEST_METHOD(GuidEmpty_string_AllZero)
		{
			Assert::AreEqual(std::string("00000000-0000-0000-0000-000000000000"), static_cast<std::string>(Guid::Empty));
		}

		TEST_METHOD(EmptyGuid_wstring_AllZero)
		{
			Guid emptyGuid;

			Assert::AreEqual(std::wstring(L"00000000-0000-0000-0000-000000000000"), static_cast<std::wstring>(emptyGuid));
		}

		TEST_METHOD(EmptyGuid_string_AllZero)
		{
			Guid emptyGuid;

			Assert::AreEqual(std::string("00000000-0000-0000-0000-000000000000"), static_cast<std::string>(emptyGuid));
		}

		TEST_METHOD(Parse_string_works_with_DefaultFormat)
		{
			Guid emptyGuid = Guid::Parse("00000000-0000-0000-0000-000000000000");
		}

		TEST_METHOD(Parse_string_works_with_RegistryFormat)
		{
			Guid emptyGuid = Guid::Parse("{00000000-0000-0000-0000-000000000000}");
		}

		TEST_METHOD(Parse_wstring_works_with_DefaultFormat)
		{
			Guid emptyGuid = Guid::Parse(L"00000000-0000-0000-0000-000000000000");
		}

		TEST_METHOD(Parse_wstring_works_with_RegistryFormat)
		{
			Guid emptyGuid = Guid::Parse(L"{00000000-0000-0000-0000-000000000000}");
		}

		TEST_METHOD(Format_Guid_to_String)
		{
			// Arrange
			constexpr const Guid a_guid = GUID{ 0xe11b7641, 0x3a9b, 0x4d9e, { 0x9c, 0xb7, 0x72, 0xd8, 0x5b, 0xd, 0x81, 0xfd } }; // {E11B7641-3A9B-4D9E-9CB7-72D85B0D81FD}

			// Act
			const std::string value = std::format("{0}", a_guid);

			// Assert
			Assert::AreEqual("E11B7641-3A9B-4D9E-9CB7-72D85B0D81FD", value.c_str());
		}

		TEST_METHOD(Format_Guid_to_WString)
		{
			// Arrange
			constexpr const Guid a_guid = GUID{ 0xe127731e, 0x5991, 0x46b3, { 0xbd, 0xdd, 0x36, 0xf6, 0xa8, 0x43, 0x24, 0x85 } }; // {E127731E-5991-46B3-BDDD-36F6A8432485}

			// Act
			const std::wstring value = std::format(L"{0}", a_guid);

			// Assert
			Assert::AreEqual(L"E127731E-5991-46B3-BDDD-36F6A8432485", value.c_str());
		}
	};
}
