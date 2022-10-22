#include "CppUnitTest.h"

#include "InvalidOperationException.h"

using namespace Native;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace NativeTests
{
	TEST_CLASS(InvalidOperationExceptionTests)
	{
	public:
		TEST_METHOD(InvalidOperationException_ShouldBeThrowable)
		{
			try
			{
				throw InvalidOperationException("Random text...");

				Assert::Fail(L"InvalidOperationException did not throw!");
			}
			catch (const std::exception&)
			{
				// exception successfully thrown
			}
		}

		TEST_METHOD(What_ShouldReturnTheExceptionMessage)
		{
			const char* message = "This is the exception message.";

			InvalidOperationException ex(message);

			Assert::AreEqual(message, ex.what());
		}
	};
}
