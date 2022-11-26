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
	};
}
