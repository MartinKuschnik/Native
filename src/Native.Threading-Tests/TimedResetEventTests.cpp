#include "CppUnitTest.h"

#include <format>

#include "CancellationTokenSource.h"
#include "TimedResetEvent.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace std::chrono_literals;

using namespace Native::Threading;

namespace NativeThreadingTests
{
	TEST_CLASS(TimedResetEventTests)
	{
	public:

		TEST_METHOD(initial_state_is_false)
		{
			const TimedResetEvent event(1h);

			Assert::IsFalse(event.wait_one(0ms));
		}

		template <typename T>
		void state_changes_after_X_delay(T x)
		{
			const auto start = std::chrono::high_resolution_clock::now();

			const TimedResetEvent event(x);

			Assert::IsFalse(event.wait_one(0ms), L"Handle must not be set instantly after creation.");
			
			Assert::IsTrue(event.wait_one(x + 5ms), std::format(L"Handle not set after {0}", x + 5ms).c_str());

			const auto time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start);

			Assert::IsTrue((time + 1ms) >= x, std::format(L"Taken time ({0}) less than expected ({1})", time, x).c_str());
		}

		TEST_METHOD(state_changes_after_1s_delay)
		{
			state_changes_after_X_delay(1s);
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

		template <typename T>
		void state_changes_at_now_plus_X(T x)
		{
			const auto start = std::chrono::high_resolution_clock::now();

			const TimedResetEvent event(std::chrono::system_clock::now() + x);

			Assert::IsFalse(event.wait_one(0ms), L"Handle must not be set instantly after creation.");

			Assert::IsTrue(event.wait_one(x + 10ms), std::format(L"Handle not set after {0}", x + 10ms).c_str());

			const auto time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start);

			Assert::IsTrue((time + 1ms) >= x, std::format(L"Taken time ({0}) less than expected ({1})", time, x).c_str());
		}

		TEST_METHOD(state_changes_at_now_plus_1s)
		{
			state_changes_at_now_plus_X(1s);
		}

		TEST_METHOD(state_changes_at_now_plus_250ms)
		{
			state_changes_at_now_plus_X(250ms);
		}

		TEST_METHOD(state_changes_at_now_plus_100ms)
		{
			state_changes_at_now_plus_X(100ms);
		}

		TEST_METHOD(state_changes_at_now_plus_50ms)
		{
			state_changes_at_now_plus_X(50ms);
		}

		TEST_METHOD(state_changes_at_now_plus_10ms)
		{
			state_changes_at_now_plus_X(10ms);
		}

		TEST_METHOD(wait_one_returns_false_when_cancellation_token_gets_cancelled)
		{
			CancellationTokenSource cts;
			const TimedResetEvent event(1h);

			cts.cancel();

			Assert::IsFalse(event.wait_one(cts.Token));
		}

		TEST_METHOD(wait_one_returns_true_also_if_cancellation_token_does_not_get_cancelled)
		{
			CancellationTokenSource cts;
			const TimedResetEvent event(50ms);

			Assert::IsTrue(event.wait_one(cts.Token));
		}
	};
}
