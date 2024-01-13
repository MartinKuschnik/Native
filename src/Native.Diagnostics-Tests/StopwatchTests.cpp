#include "CppUnitTest.h"

#include <thread>

#include "Stopwatch.h"

using namespace std::chrono_literals;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace Native::Diagnostics;

namespace NativeDiagnosticsTests
{
	TEST_CLASS(StopwatchTests)
	{
	public:

		TEST_METHOD(Stopwatch_elapsed_s)
		{
			Stopwatch stopwatch = Stopwatch::StartNew();

			std::this_thread::sleep_for(1s);

			const std::chrono::seconds elapsed_s = stopwatch.elapsed<std::chrono::seconds>();

			Assert::IsTrue(elapsed_s >= 1s);
		}
		
		TEST_METHOD(Stopwatch_elapsed_ms)
		{
			Stopwatch stopwatch = Stopwatch::StartNew();

			std::this_thread::sleep_for(100ms);

			const std::chrono::milliseconds elapsed_ms = stopwatch.elapsed<std::chrono::milliseconds>();

			Assert::IsTrue(elapsed_ms >= 100ms);
		}

		TEST_METHOD(Stopwatch_elapsed_ns)
		{
			Stopwatch stopwatch = Stopwatch::StartNew();

			std::this_thread::sleep_for(1000ns);

			const std::chrono::nanoseconds elapsed_ns = stopwatch.elapsed<std::chrono::nanoseconds>();

			Assert::IsTrue(elapsed_ns >= 1000ns);
		}
	};
}
