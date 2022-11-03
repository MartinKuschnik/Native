#include "CppUnitTest.h"

#include "Win32Exception.h"

using namespace Native::Windows;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace NativeWindowsTests
{
	TEST_CLASS(Win32ExceptionTests)
	{
	public:
		TEST_METHOD(Win32Exception_ShouldBeThrowable)
		{
			try
			{
				throw Win32Exception(123, nameof(ShouldBeThrowable));
			}
			catch (const std::exception&)
			{
				// exception successfully thrown
			}
		}

		TEST_METHOD(What_ShouldReturnTheCorrectExceptionMessage)
		{
			const char* expactedMessage = "ShouldBeThrowable(...) failed! (Win32ErrorCode=0x0000007B: The filename, directory name, or volume label syntax is incorrect.)";

			Win32Exception ex(0x0000007B, nameof(ShouldBeThrowable));

			Assert::AreEqual(expactedMessage, ex.what());
		}
	};
}
