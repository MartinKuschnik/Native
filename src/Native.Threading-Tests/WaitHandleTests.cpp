#include "CppUnitTest.h"

#include "CancellationTokenSource.h"
#include "ManualResetEvent.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


using namespace std::chrono_literals;
using namespace Native::Threading;

namespace NativeThreadingTests
{
	TEST_CLASS(WaitHandleTests)
	{
	public:

		TEST_METHOD(WaitAll_with_two_not_signaled_wait_handles)
		{
			// Arrange
			const std::shared_ptr<ManualResetEvent> mre1 = std::make_shared<ManualResetEvent>(false);
			const std::shared_ptr<ManualResetEvent> mre2 = std::make_shared<ManualResetEvent>(false);

			const std::array<const std::shared_ptr<WaitHandle>, 2> wait_handles { mre1 , mre2 };

			const CancellationTokenSource cts(1s);

			// Act
			const bool result = WaitHandle::WaitAll(wait_handles, cts.Token);

			// Assert
			Assert::IsFalse(result);
		}

		TEST_METHOD(WaitAll_with_first_not_signaled_wait_handles)
		{
			// Arrange
			const std::shared_ptr<ManualResetEvent> mre1 = std::make_shared<ManualResetEvent>(false);
			const std::shared_ptr<ManualResetEvent> mre2 = std::make_shared<ManualResetEvent>(true);

			const std::array<const std::shared_ptr<WaitHandle>, 2> wait_handles{ mre1 , mre2 };

			const CancellationTokenSource cts(1s);

			// Act
			const bool result = WaitHandle::WaitAll(wait_handles, cts.Token);

			// Assert
			Assert::IsFalse(result);
		}

		TEST_METHOD(WaitAll_with_second_not_signaled_wait_handles)
		{
			// Arrange
			const std::shared_ptr<ManualResetEvent> mre1 = std::make_shared<ManualResetEvent>(true);
			const std::shared_ptr<ManualResetEvent> mre2 = std::make_shared<ManualResetEvent>(false);

			const std::array<const std::shared_ptr<WaitHandle>, 2> wait_handles{ mre1 , mre2 };

			const CancellationTokenSource cts(1s);

			// Act
			const bool result = WaitHandle::WaitAll(wait_handles, cts.Token);

			// Assert
			Assert::IsFalse(result);
		}

		TEST_METHOD(WaitAll_with_two_signaled_wait_handles)
		{
			// Arrange
			const std::shared_ptr<ManualResetEvent> mre1 = std::make_shared<ManualResetEvent>(true);
			const std::shared_ptr<ManualResetEvent> mre2 = std::make_shared<ManualResetEvent>(true);

			const std::array<const std::shared_ptr<WaitHandle>, 2> wait_handles{ mre1 , mre2 };

			// Act
			const bool result = WaitHandle::WaitAll(wait_handles);

			// Assert
			Assert::IsTrue(result);
		}

		TEST_METHOD(WaitAll_with_CancellationToken_None)
		{
			// Arrange
			const std::shared_ptr<ManualResetEvent> mre1 = std::make_shared<ManualResetEvent>(true);
			const std::shared_ptr<ManualResetEvent> mre2 = std::make_shared<ManualResetEvent>(true);

			const std::array<const std::shared_ptr<WaitHandle>, 2> wait_handles{ mre1 , mre2 };

			// Act
			const bool result = WaitHandle::WaitAll(wait_handles, CancellationToken::None);

			// Assert
			Assert::IsTrue(result);
		}

		TEST_METHOD(WaitAny_with_two_not_signaled_wait_handles)
		{
			// Arrange
			const std::shared_ptr<ManualResetEvent> mre1 = std::make_shared<ManualResetEvent>(false);
			const std::shared_ptr<ManualResetEvent> mre2 = std::make_shared<ManualResetEvent>(false);

			const std::array<const std::shared_ptr<WaitHandle>, 2> wait_handles{ mre1 , mre2 };

			const CancellationTokenSource cts(1s);

			// Act
			const std::optional<uint8_t> result = WaitHandle::WaitAny(wait_handles, cts.Token);

			// Assert
			Assert::IsFalse(result.has_value());
		}

		TEST_METHOD(WaitAny_with_first_not_signaled_wait_handles)
		{
			// Arrange
			const std::shared_ptr<ManualResetEvent> mre1 = std::make_shared<ManualResetEvent>(false);
			const std::shared_ptr<ManualResetEvent> mre2 = std::make_shared<ManualResetEvent>(true);

			const std::array<const std::shared_ptr<WaitHandle>, 2> wait_handles{ mre1 , mre2 };

			// Act
			const uint8_t result = WaitHandle::WaitAny(wait_handles);

			// Assert
			Assert::AreEqual(1ui8, result);
		}

		TEST_METHOD(WaitAny_with_second_not_signaled_wait_handles)
		{
			// Arrange
			const std::shared_ptr<ManualResetEvent> mre1 = std::make_shared<ManualResetEvent>(true);
			const std::shared_ptr<ManualResetEvent> mre2 = std::make_shared<ManualResetEvent>(false);

			const std::array<const std::shared_ptr<WaitHandle>, 2> wait_handles{ mre1 , mre2 };

			// Act
			const uint8_t result = WaitHandle::WaitAny(wait_handles);

			// Assert
			Assert::AreEqual(0ui8, result);
		}

		TEST_METHOD(WaitAny_with_CancellationToken_None)
		{
			// Arrange
			const std::shared_ptr<ManualResetEvent> mre1 = std::make_shared<ManualResetEvent>(true);
			const std::shared_ptr<ManualResetEvent> mre2 = std::make_shared<ManualResetEvent>(true);

			const std::array<const std::shared_ptr<WaitHandle>, 2> wait_handles{ mre1 , mre2 };

			// Act
			const std::optional<uint8_t> result = WaitHandle::WaitAny(wait_handles, CancellationToken::None);

			// Assert
			Assert::IsTrue(result.has_value());
		}

		TEST_METHOD(WaitAny_with_Timebased_CancellationToken)
		{
			// Arrange
			const std::shared_ptr<ManualResetEvent> mre1 = std::make_shared<ManualResetEvent>(false);
			const std::shared_ptr<ManualResetEvent> mre2 = std::make_shared<ManualResetEvent>(false);

			const std::array<const std::shared_ptr<WaitHandle>, 2> wait_handles{ mre1 , mre2 };

			CancellationTokenSource cts(10ms);

			// Act
			const std::optional<uint8_t> result = WaitHandle::WaitAny(wait_handles, cts.Token);

			// Assert
			Assert::IsFalse(result.has_value());
		}

		TEST_METHOD(WaitAny_with_Canceled_CancellationToken)
		{
			// Arrange
			const std::shared_ptr<ManualResetEvent> mre1 = std::make_shared<ManualResetEvent>(false);
			const std::shared_ptr<ManualResetEvent> mre2 = std::make_shared<ManualResetEvent>(false);

			const std::array<const std::shared_ptr<WaitHandle>, 2> wait_handles{ mre1 , mre2 };

			CancellationTokenSource cts;
			cts.cancel();

			// Act
			const std::optional<uint8_t> result = WaitHandle::WaitAny(wait_handles, cts.Token);

			// Assert
			Assert::IsFalse(result.has_value());
		}
	};
}
