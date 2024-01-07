#include "CppUnitTest.h"

#include "HResultException.h"

using namespace Native::Windows;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace NativeWindowsTests
{
	TEST_CLASS(HResultExceptionTests)
	{
	public:
		TEST_METHOD(HResultException_ShouldBeThrowable)
		{
			try
			{
				throw HResultException(123, nameof(ShouldBeThrowable));
			}
			catch (const std::exception&)
			{
				// exception successfully thrown
			}
		}

		TEST_METHOD(What_ShouldContainTheExceptionMessage)
		{
			const char* expactedMessage = "ShouldBeThrowable(...) failed! (HRESULT=0x0000007B: The filename, directory name, or volume label syntax is incorrect.)";

			HResultException ex(0x0000007B, nameof(ShouldBeThrowable));

			Assert::AreNotEqual(std::string_view(ex.what()).find(expactedMessage), std::string::npos);
		}
	};
}
