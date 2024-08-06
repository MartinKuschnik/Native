#include "CppUnitTest.h"

#include <thread>

#include "Lazy.h"
#include "Parallel.h"

using namespace std::chrono_literals;

using namespace Native;
using namespace Native::Threading;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace NativeTests
{
	TEST_CLASS(LazyTests)
	{
	private:

	public:

		TEST_METHOD(value_is_created_after_first_value_call)
		{
			bool value_factory_called = false;

			const auto value_factory = [&value_factory_called]() {			
				value_factory_called = true;
				return -99;			
			};

			const Lazy<int> lazy(value_factory);

			Assert::IsFalse(value_factory_called);

			int value = lazy.value();

			Assert::AreEqual(-99, value);

			Assert::IsTrue(value_factory_called);
		}

		TEST_METHOD(lazy_with_thread_safety_mode_none_is_not_thread_safe)
		{
			int value_factory_calls = 0;

			const auto value_factory = [&value_factory_calls]() {
				int val = ++value_factory_calls;
				std::this_thread::sleep_for(100ms * val);
				return val;
			};

			const Lazy<int> lazy(value_factory, LazyThreadSafetyMode::None);

			Assert::AreEqual(0, value_factory_calls);

			auto val1 = Tasks::Task<int>::Run([&lazy] { return lazy.value(); });
			auto val2 = Tasks::Task<int>::Run([&lazy] { return lazy.value(); });
			auto val3 = Tasks::Task<int>::Run([&lazy] { return lazy.value(); });

			Assert::AreEqual(1, val1.result());
			Assert::AreEqual(2, val2.result());
			Assert::AreEqual(3, val3.result());

			Assert::AreEqual(3, value_factory_calls);
		}

		TEST_METHOD(lazy_with_thread_safety_mode_publication_only_is_partially_thread_safe)
		{
			int value_factory_calls = 0;

			const auto value_factory = [&value_factory_calls]() {
				int val = ++value_factory_calls;
				std::this_thread::sleep_for(100ms * val);
				return val;
			};

			const Lazy<int> lazy(value_factory, LazyThreadSafetyMode::PublicationOnly);

			Assert::AreEqual(0, value_factory_calls);

			auto val1 = Tasks::Task<int>::Run([&lazy] { return lazy.value(); });
			auto val2 = Tasks::Task<int>::Run([&lazy] { return lazy.value(); });
			auto val3 = Tasks::Task<int>::Run([&lazy] { return lazy.value(); });

			Assert::AreEqual(1, val1.result(), L"first");
			Assert::AreEqual(1, val2.result(), L"second");
			Assert::AreEqual(1, val3.result(), L"last");

			Assert::AreEqual(3, value_factory_calls);
		}

		TEST_METHOD(lazy_with_thread_safety_mode_execution_and_publication_is_thread_safe)
		{
			int value_factory_calls = 0;

			const auto value_factory = [&value_factory_calls]() {
				int val = ++value_factory_calls;
				std::this_thread::sleep_for(100ms * val);
				return val;
			};

			const Lazy<int> lazy(value_factory, LazyThreadSafetyMode::ExecutionAndPublication);

			Assert::AreEqual(0, value_factory_calls);

			auto val1 = Tasks::Task<int>::Run([&lazy] { return lazy.value(); });
			auto val2 = Tasks::Task<int>::Run([&lazy] { return lazy.value(); });
			auto val3 = Tasks::Task<int>::Run([&lazy] { return lazy.value(); });

			Assert::AreEqual(1, val1.result());
			Assert::AreEqual(1, val2.result());
			Assert::AreEqual(1, val3.result());

			Assert::AreEqual(1, value_factory_calls);
		}

		TEST_METHOD(lazy_with_is_thread_false_is_not_thread_safe)
		{
			int value_factory_calls = 0;

			const auto value_factory = [&value_factory_calls]() {
				int val = ++value_factory_calls;
				std::this_thread::sleep_for(100ms * val);
				return val;
			};

			const Lazy<int> lazy(value_factory, false);

			Assert::AreEqual(0, value_factory_calls);

			auto val1 = Tasks::Task<int>::Run([&lazy] { return lazy.value(); });
			auto val2 = Tasks::Task<int>::Run([&lazy] { return lazy.value(); });
			auto val3 = Tasks::Task<int>::Run([&lazy] { return lazy.value(); });

			Assert::AreEqual(1, val1.result());
			Assert::AreEqual(2, val2.result());
			Assert::AreEqual(3, val3.result());

			Assert::AreEqual(3, value_factory_calls);
		}

		TEST_METHOD(lazy_with_is_thread_true_is_thread_safe)
		{
			int value_factory_calls = 0;

			const auto value_factory = [&value_factory_calls]() {
				int val = ++value_factory_calls;
				std::this_thread::sleep_for(100ms * val);
				return val;
			};

			const Lazy<int> lazy(value_factory, true);

			Assert::AreEqual(0, value_factory_calls);

			auto val1 = Tasks::Task<int>::Run([&lazy] { return lazy.value(); });
			auto val2 = Tasks::Task<int>::Run([&lazy] { return lazy.value(); });
			auto val3 = Tasks::Task<int>::Run([&lazy] { return lazy.value(); });

			Assert::AreEqual(1, val1.result());
			Assert::AreEqual(1, val2.result());
			Assert::AreEqual(1, val3.result());

			Assert::AreEqual(1, value_factory_calls);
		}
	};
}
