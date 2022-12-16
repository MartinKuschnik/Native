#include "CppUnitTest.h"

#include "ManualResetEvent.h"

#include "CancellationTokenSource.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace std::chrono;
using namespace std::chrono_literals;

using namespace Native::Threading;

namespace NativeThreadingTests
{
	TEST_CLASS(CancellationTokenSourceTests)
	{
	public:

		TEST_METHOD(token_gets_canceled_after_cancel)
		{
			CancellationTokenSource cancellationTokenSource;
			const CancellationToken token = cancellationTokenSource.token();

			Assert::IsFalse(token.is_cancellation_requested());

			cancellationTokenSource.cancel();

			Assert::IsTrue(token.is_cancellation_requested());
		}

		TEST_METHOD(token_with_delay_can_be_canceled_earlier)
		{
			CancellationTokenSource cancellationTokenSource(1h);
			const CancellationToken token = cancellationTokenSource.token();

			Assert::IsFalse(token.is_cancellation_requested());

			cancellationTokenSource.cancel();

			Assert::IsTrue(token.is_cancellation_requested());
		}

		TEST_METHOD(token_with_delay_gets_canceled_after_delay)
		{
			const CancellationTokenSource cancellationTokenSource(100ms);
			const CancellationToken token = cancellationTokenSource.token();

			Assert::IsFalse(token.is_cancellation_requested());
			std::this_thread::sleep_for(50ms);
			Assert::IsFalse(token.is_cancellation_requested());

			std::this_thread::sleep_for(50ms);

			Assert::IsTrue(token.is_cancellation_requested());
		}

		TEST_METHOD(token_with_due_time_can_be_canceled_earlier)
		{
			const system_clock::time_point due_time = system_clock::now() + 1h;

			CancellationTokenSource cancellationTokenSource(due_time);
			const CancellationToken token = cancellationTokenSource.token();

			Assert::IsFalse(token.is_cancellation_requested());

			cancellationTokenSource.cancel();

			Assert::IsTrue(token.is_cancellation_requested());
		}

		TEST_METHOD(token_with_due_time_gets_canceled_at_due_time)
		{
			const system_clock::time_point due_time = system_clock::now() + 100ms;

			const CancellationTokenSource cancellationTokenSource(due_time);
			const CancellationToken token = cancellationTokenSource.token();

			Assert::IsFalse(token.is_cancellation_requested());
			std::this_thread::sleep_for(50ms);
			Assert::IsFalse(token.is_cancellation_requested());

			std::this_thread::sleep_for(50ms);

			Assert::IsTrue(token.is_cancellation_requested());
		}
	};
}
