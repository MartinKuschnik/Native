#include "CppUnitTest.h"

#include <chrono>

#include "Event.h"

using namespace Native;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace NativeTests
{
	TEST_CLASS(EventTests)
	{
	private:

		class TestEventArgs { };

		using TestEvent = Event<TestEventArgs, EventTests>;

	public:
		TEST_METHOD(FunctionCallOperator_CallsSubscribedFunction)
		{
			bool eventHandlerCalled = false;

			TestEvent event;

			TestEvent::Subscription subscription = event.subscribe([&](auto& args) { eventHandlerCalled = true; });

			event(TestEventArgs());

			Assert::AreEqual(true, eventHandlerCalled);
		}

		TEST_METHOD(FunctionCallOperator_CallsEverySubscribedFunction)
		{
			bool eventHandlerOneCalled = false, eventHandlerTwoCalled = false;

			TestEvent event;

			TestEvent::Subscription subscriptionOne = event.subscribe([&](auto& args) { eventHandlerOneCalled = true; });
			TestEvent::Subscription subscriptionTwo = event.subscribe([&](auto& args) { eventHandlerTwoCalled = true; });

			event(TestEventArgs());

			Assert::AreEqual(true, eventHandlerOneCalled);
			Assert::AreEqual(true, eventHandlerTwoCalled);
		}

		TEST_METHOD(FunctionCallOperator_DoesNotCallDeadSubscribedFunction)
		{
			bool eventHandlerCalled = false;

			TestEvent event;

			{
				TestEvent::Subscription subscription = event.subscribe([&](auto& args) { eventHandlerCalled = true; });
			}

			event(TestEventArgs());

			Assert::AreEqual(false, eventHandlerCalled);
		}

		TEST_METHOD(FunctionCallOperator_CallsEverySubscribedFunctionInSubscripedOrder)
		{
			using namespace std::chrono;
			using time_point = high_resolution_clock::time_point;

			time_point eventHandlerOneCalledAt, eventHandlerTwoCalledAt;

			TestEvent event;

			TestEvent::Subscription subscriptionOne = event.subscribe([&](auto& args) {
					eventHandlerOneCalledAt = high_resolution_clock::now();
					std::this_thread::sleep_for(std::chrono::milliseconds(1));
				});
			
			TestEvent::Subscription subscriptionTwo = event.subscribe([&](auto& args) {
					eventHandlerTwoCalledAt = high_resolution_clock::now();
					std::this_thread::sleep_for(std::chrono::milliseconds(1));
				});

			event(TestEventArgs());

			Assert::IsTrue(eventHandlerOneCalledAt < eventHandlerTwoCalledAt);
		}
	};
}
