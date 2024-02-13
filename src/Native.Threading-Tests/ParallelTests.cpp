#include "CppUnitTest.h"

#include <atomic>

#include "Parallel.h"

#include <AggregateException.h>
#include "CancellationToken.h"
#include "CancellationTokenSource.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace std::chrono_literals;

using namespace Native;
using namespace Native::Threading;
using namespace Native::Threading::Tasks;

namespace NativeThreadingTests
{
	TEST_CLASS(ParallelTests)
	{
	public:

		struct TestException : public std::exception {};

		static std::experimental::generator<int> GenerateInts(const int count)
		{
			for (int i = 0; i < count; i++)
			{
				co_yield i;
			}
		}

		template<typename T>
		struct SaticForEachCallback
		{
			static inline std::atomic<uint32_t> Calls;

			static void Callback(T param)
			{
				Calls++;
			}
		};

		template<typename T>
		struct NonSaticForEachCallback
		{
			std::shared_ptr<std::atomic<uint32_t>> Calls;

			void Callback(T param)
			{
				(*Calls)++;
			}
		};

		TEST_METHOD(ForEach_works_with_lambda_expression)
		{
			// Arrange
			constexpr int amount_of_numbers = 100;

			const ParallelOptions parallel_options;

			std::atomic<uint32_t> calls;

			// Act
			Parallel::ForEach(GenerateInts(amount_of_numbers), parallel_options, [&calls](int i) {
				calls++;
			});

			// Assert
			Assert::AreEqual(amount_of_numbers, static_cast<int>(calls));
		}

		TEST_METHOD(ForEach_works_with_static_method)
		{
			// Arrange
			constexpr int amount_of_numbers = 100;

			const ParallelOptions parallel_options;

			SaticForEachCallback<int>::Calls = 0;

			// Act
			Parallel::ForEach(GenerateInts(amount_of_numbers), parallel_options, &SaticForEachCallback<int>::Callback);

			// Assert
			Assert::AreEqual(amount_of_numbers, static_cast<int>(SaticForEachCallback<int>::Calls));
		}

		TEST_METHOD(ForEach_works_with_non_static_method)
		{
			// Arrange
			constexpr int amount_of_numbers = 100;

			const ParallelOptions parallel_options;

			NonSaticForEachCallback<int> callback{ std::make_shared<std::atomic<uint32_t>>(0) };

			// Act
			Parallel::ForEach(GenerateInts(amount_of_numbers), parallel_options, &NonSaticForEachCallback<int>::Callback, callback);

			// Assert
			Assert::AreEqual(amount_of_numbers, static_cast<int>(*callback.Calls));
		}

		TEST_METHOD(ForEach_rethrows_exceptions)
		{
			// Arrange
			const ParallelOptions parallel_options;

			// Act & Assert

			Assert::ExpectException<AggregateException>([parallel_options]() {

				Parallel::ForEach(GenerateInts(100), parallel_options, [](int i) {
					throw TestException();
				});

			});
		}

		TEST_METHOD(ForEach_does_not_exceed_MaxDegreeOfParallelism)
		{
			// Arrange
			const ManualResetEvent mre(false);

			int calls = 0;

			CancellationTokenSource cts(500ms);

			const ParallelOptions parallel_options { .CancellationToken = cts.Token, .MaxDegreeOfParallelism = 2 };

			// Act
			Parallel::ForEach(GenerateInts(100), parallel_options, [&calls, &mre, ct = cts.Token](int i) {

				calls++;
				mre.wait_one(ct);

			});

			// Assert
			Assert::AreEqual(2, calls);
		}
	};
}