#include "CppUnitTest.h"

#include "ManualResetEvent.h"

#include "CancellationToken.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace Native::Threading;

namespace NativeThreadingTests
{
	TEST_CLASS(CancellationTokenTests)
	{
	public:
		
		TEST_METHOD(is_cancellation_requested_For_Default_CancellationToken_Is_False)
		{
			const CancellationToken token;

			Assert::IsFalse(token.is_cancellation_requested());
		}

		TEST_METHOD(is_cancellation_requested_For_Signaled_WaitHandle_Is_True)
		{
			auto signaledWaitHandel = std::make_shared<const ManualResetEvent>(true);

			const CancellationToken token(signaledWaitHandel);

			Assert::IsTrue(token.is_cancellation_requested());
		}

		TEST_METHOD(is_cancellation_requested_For_Not_Signaled_WaitHandle_Is_False)
		{
			const auto notSignaledWaitHandel = std::make_shared<const ManualResetEvent>(false);

			const CancellationToken token(notSignaledWaitHandel);

			Assert::IsFalse(token.is_cancellation_requested());
		}

		TEST_METHOD(wait_handle_Returns_The_Provided_WaitHandel)
		{
			const auto notSignaledWaitHandel = std::make_shared<const ManualResetEvent>(false);

			const CancellationToken token(notSignaledWaitHandel);

			const WaitHandle& waitHandel = token.wait_handle();

			Assert::AreEqual(dynamic_cast<const void*>(notSignaledWaitHandel.get()), dynamic_cast<const void*>(&waitHandel));
		}
	};
}
