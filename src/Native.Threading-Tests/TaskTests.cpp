#include "CppUnitTest.h"

#include "Task.h"

#include "AggregateException.h"
#include "CancellationToken.h"
#include "CancellationTokenSource.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace std::chrono_literals;

using namespace Native;
using namespace Native::Threading;
using namespace Native::Threading::Tasks;

namespace NativeThreadingTests
{
	TEST_CLASS(TaskTests)
	{
	public:

		struct TestException : public std::exception {};

		TEST_METHOD(Task_runs_in_parallel)
		{
			// Arrange
			int task_thread_id = 0;

			// Act
			const Task<std::thread::id> task = Task<std::thread::id>::Run([]() {
				return std::this_thread::get_id();
			});

			task.wait();

			// Assert

			Assert::IsTrue(std::this_thread::get_id() != task.result());
		}

		TEST_METHOD(wait_throws_occurred_exception)
		{
			// Arrange
			const Task<void> task = Task<void>::Run([]() {
				throw TestException();
			});

			// Act & Assert
			Assert::ExpectException<TestException>([task]() { task.wait(); });
		}

		TEST_METHOD(wait_with_CancellationToken_throws_occurred_exception)
		{
			// Arrange
			const Task<void> task = Task<void>::Run([]() {
				throw TestException();
				});

			const CancellationTokenSource cts;
			const CancellationToken ct = cts.Token;

			// Act & Assert
			Assert::ExpectException<TestException>([task, ct]() { task.wait(ct); });
		}

		TEST_METHOD(result_throws_occurred_exception)
		{
			// Arrange
			const Task<void> task = Task<void>::Run([]() {
				throw TestException();
				});

			// Act & Assert
			Assert::ExpectException<TestException>([task]() { task.result(); });
		}

		TEST_METHOD(WaitAll_rethrows_occurred_exception_as_AggregateException)
		{
			// Arrange
			const Task<void> task1 = Task<void>::Run([]() {
				throw TestException();
				});

			const Task<void> task2 = Task<void>::Run([]() {
				throw TestException();
				});

			const std::array<Task<void>, 2> tasks { task1 ,task2 };

			// Act & Assert
			Assert::ExpectException<AggregateException>([tasks]() { Task<>::WaitAll(tasks); });
		}

		TEST_METHOD(WaitAny_does_not_rethrows_occurred_exception)
		{
			// Arrange
			const Task<void> task1 = Task<void>::Run([]() {
				throw TestException();
				});

			const Task<void> task2 = Task<void>::Run([]() {
				throw TestException();
				});

			const std::array<Task<void>, 2> tasks{ task1 ,task2 };

			// Act
			Task<>::WaitAny(tasks);
		}

		TEST_METHOD(Failed_is_false_on_success)
		{
			// Arrange
			const Task<void> task = Task<void>::Run([]() { });

			// Act
			Task<>::WaitAny(task); // use WaitAny instead of wait to avoid throwing an occurred exception

			// Assert
			Assert::IsFalse(task.failed());
		}

		TEST_METHOD(Failed_is_true_on_failure)
		{
			// Arrange
			const Task<void> task = Task<void>::Run([]() { throw TestException(); });

			// Act
			Task<>::WaitAny(task); // use WaitAny instead of wait to avoid throwing an occurred exception

			// Assert
			Assert::IsTrue(task.failed());
		}
	};
}
