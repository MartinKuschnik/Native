#include "CppUnitTest.h"

#include "ArgumentException.h"

using namespace Native;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace NativeTests
{
	TEST_CLASS(ArgumentExceptionTests)
	{
	public:
		TEST_METHOD(ArgumentException_ShouldBeThrowable)
		{
			try
			{
				throw ArgumentException("Anything is not supported.");

				Assert::Fail(L"ArgumentException did not throw!");
			}
			catch (const std::exception&)
			{
				// exception successfully thrown
			}
		}

		TEST_METHOD(What_ShouldReturnTheExceptionMessage)
		{
			const char* message = "This is the exception message!";

			ArgumentException ex(message);

			Assert::AreEqual(message, ex.what());
		}
	};
}
