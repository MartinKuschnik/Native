#include "CppUnitTest.h"

#include "FormatException.h"

using namespace Native;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace NativeTests
{
	TEST_CLASS(FormatExceptionTests)
	{
	public:
		TEST_METHOD(FormatException_ShouldBeThrowable)
		{
			try
			{
				throw FormatException("Random text...");

				Assert::Fail(L"FormatException did not throw!");
			}
			catch (const std::exception&)
			{
				// exception successfully thrown
			}
		}

		TEST_METHOD(What_ShouldReturnTheExceptionMessage)
		{
			const char* message = "This is the exception message.";

			FormatException ex(message);

			Assert::AreEqual(message, ex.what());
		}
	};
}
