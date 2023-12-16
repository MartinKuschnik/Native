#include "CppUnitTest.h"

#include <thread>

#include "AsyncCompletionSource.h"
#include "CancellationTokenSource.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace Native::Threading;

using namespace std::chrono_literals;

namespace NativeThreadingTests
{
	TEST_CLASS(AsyncCompletionTests)
	{
	public:

		TEST_METHOD(uncompleted_blocks)
		{
			AsyncCompletionSource<int> acs;

			Assert::IsFalse(acs.WaitHandle->wait_one(100ms));
		}

		TEST_METHOD(immediate_completion_does_not_block)
		{
			constexpr int result = 123;

			AsyncCompletionSource<int> acs;

			acs.complete(result);

			Assert::IsTrue(acs.WaitHandle->wait_one(0ms));

			Assert::AreEqual(result, *acs.WaitHandle->result());
		}

		TEST_METHOD(delayed_completion_blocks_a_while)
		{
			constexpr int result = 123;

			AsyncCompletionSource<int> acs;

			std::thread thread([&acs]() {
				std::this_thread::sleep_for(50ms);
				acs.complete(result);
			});

			bool completed = acs.WaitHandle->wait_one(100ms);

			thread.join();

			Assert::IsTrue(completed);

			Assert::AreEqual(result, *acs.WaitHandle->result());
		}
	};
}
