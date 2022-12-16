#include "CppUnitTest.h"

#include <random>

#include "CancellationTokenSource.h"
#include "MultiWaitHandle.h"
#include "ManualResetEvent.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace std::chrono_literals;

using namespace Native::Threading;

namespace NativeThreadingTests
{
	TEST_CLASS(MultiWaitHandleTests)
	{
	public:

		TEST_METHOD(state_changes_after_first_wait_handle_signals)
		{
			const std::shared_ptr<ManualResetEvent> one = std::make_shared<ManualResetEvent>(false), two = std::make_shared<ManualResetEvent>(false);

			const MultiWaitHandle handle(one, two);

			Assert::IsFalse(handle.wait_one(0ms));

			one->set();

			Assert::IsTrue(handle.wait_one(0ms));
		}

		TEST_METHOD(state_changes_after_second_wait_handle_signals)
		{
			const std::shared_ptr<ManualResetEvent> one = std::make_shared<ManualResetEvent>(false), two = std::make_shared<ManualResetEvent>(false);

			const MultiWaitHandle handle(one, two);

			Assert::IsFalse(handle.wait_one(0ms));

			two->set();

			Assert::IsTrue(handle.wait_one(0ms));
		}



		TEST_METHOD(state_changes_after_nested_wait_handle_signals)
		{
			const std::shared_ptr<ManualResetEvent> one = std::make_shared<ManualResetEvent>(false), two = std::make_shared<ManualResetEvent>(false), tree = std::make_shared<ManualResetEvent>(false);
			std::shared_ptr<MultiWaitHandle> nestedMultiWaitHandle = std::make_shared<MultiWaitHandle>(one, two);

			const MultiWaitHandle handle(nestedMultiWaitHandle, tree);

			Assert::IsFalse(handle.wait_one(0ms));

			one->set();

			Assert::IsTrue(handle.wait_one(0ms));
		}

		TEST_METHOD(state_changes_after_any_wait_handle_signals)
		{
			std::array<std::shared_ptr<ManualResetEvent>, MAXIMUM_WAIT_OBJECTS> count_handles;

			for (size_t i = 0; i < MAXIMUM_WAIT_OBJECTS; i++)
				count_handles[i] = std::make_shared<ManualResetEvent>(false);

			const MultiWaitHandle handle(count_handles);

			Assert::IsFalse(handle.wait_one(0ms));

			{
				std::random_device random_device;
				std::mt19937 random_engine(random_device());
				std::uniform_int_distribution<int> distribution_1_100(0, MAXIMUM_WAIT_OBJECTS - 1);

				count_handles[distribution_1_100(random_engine)]->set();
			}

			Assert::IsTrue(handle.wait_one(0ms));
		}

		TEST_METHOD(wait_one_returns_false_when_cancellation_token_gets_cancelled)
		{
			CancellationTokenSource cts;
			const std::shared_ptr<ManualResetEvent> one = std::make_shared<ManualResetEvent>(false), two = std::make_shared<ManualResetEvent>(false);

			const MultiWaitHandle handle(one, two);

			cts.cancel();

			Assert::IsFalse(handle.wait_one(cts.token()));
		}

		TEST_METHOD(wait_one_returns_true_also_if_cancellation_token_does_not_get_cancelled_I)
		{
			CancellationTokenSource cts;
			const std::shared_ptr<ManualResetEvent> one = std::make_shared<ManualResetEvent>(false), two = std::make_shared<ManualResetEvent>(false);

			const MultiWaitHandle handle(one, two);

			one->set();

			Assert::IsTrue(handle.wait_one(cts.token()));
		}

		TEST_METHOD(wait_one_returns_true_also_if_cancellation_token_does_not_get_cancelled_II)
		{
			CancellationTokenSource cts;
			const std::shared_ptr<ManualResetEvent> one = std::make_shared<ManualResetEvent>(false), two = std::make_shared<ManualResetEvent>(false);

			const MultiWaitHandle handle(one, two);

			two->set();

			Assert::IsTrue(handle.wait_one(cts.token()));
		}
	};
}
