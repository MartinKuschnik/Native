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
			try
			{
				Guid::Parse(L"This is not a Guid!!!");

				Assert::Fail(L"InvalidOperationException did not throw!");
			}
			catch (const std::exception&)
			{
				// exception successfully thrown
			}
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
	};
}
