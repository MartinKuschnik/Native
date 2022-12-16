#include "CppUnitTest.h"

#include <fmt/format.h>
#include <fmt/xchar.h>
#include <fmt/chrono.h>

#include "TimedResetEvent.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace std::chrono_literals;

using namespace Native::Threading;

namespace NativeThreadingTests
{
	TEST_CLASS(TimedResetEventTests)
	{
	public:

		template <typename T>
		void state_changes_after_X_delay(T x)
		{
			const auto start = std::chrono::high_resolution_clock::now();

			const TimedResetEvent event(x);

			Assert::IsTrue(event.wait_one(x + 1ms), fmt::format(L"Handle not set after {0}", x + 2ms).c_str());

			const auto time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start);

			Assert::IsTrue((time + 1ms) >= x, fmt::format(L"Taken time ({0}) less than expected ({1})", time, x).c_str());
		}

		TEST_METHOD(initial_state_is_false)
		{
			const TimedResetEvent event(1h);

			Assert::IsFalse(event.wait_one(0ms));
		}

		TEST_METHOD(state_changes_after_1s_delay)
		{
			state_changes_after_X_delay(1s);
		}

		TEST_METHOD(state_changes_after_500ms_delay)
		{
			state_changes_after_X_delay(500ms);
		}

		TEST_METHOD(state_changes_after_250ms_delay)
		{
			state_changes_after_X_delay(250ms);
		}

		TEST_METHOD(state_changes_after_100ms_delay)
		{
			state_changes_after_X_delay(100ms);
		}

		TEST_METHOD(state_changes_after_50ms_delay)
		{
			state_changes_after_X_delay(50ms);
		}

		TEST_METHOD(state_changes_after_10ms_delay)
		{
			state_changes_after_X_delay(10ms);
		}
	};
}
