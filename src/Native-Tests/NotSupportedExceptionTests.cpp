#include "CppUnitTest.h"

#include "NotSupportedException.h"

using namespace Native;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace NativeTests
{
	TEST_CLASS(NotSupportedExceptionTests)
	{
	public:
		TEST_METHOD(NotSupportedException_ShouldBeThrowable)
		{
			try
			{
				throw NotSupportedException("Anything is not supported.");

				Assert::Fail(L"NotSupportedException did not throw!");
			}
			catch (const std::exception&)
			{
				// exception successfully thrown
			}
		}

		TEST_METHOD(What_ShouldContainTheExceptionMessage)
		{
			const char* message = "This is the exception message.";

			NotSupportedException ex(message);

			Assert::AreNotEqual(std::string_view(ex.what()).find(message), std::string::npos);
		}
	};
}
