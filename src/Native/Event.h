#pragma once

#define NODISCARD_MSG_EVENTSUBSCRIPTION_DROPED "This function creating a subscription represented by the return value. Discarding the return value will destroy the subscription instantly."

#include <functional>
#include <memory>
#include <mutex>
#include <optional>
#include <unordered_set>

namespace Native
{
	template<class TArgs, class TEventSource>
	class Event
	{
		using EventHandler = std::function<void(TArgs&)>;

#pragma region Subclasses

	private:
		class EventHandlerList;

	public:
		class Subscription
		{
		private:
			const std::shared_ptr<EventHandlerList> _eventHandlerList;

		public:
			const std::shared_ptr<EventHandler> Target;

			Subscription(const std::shared_ptr<EventHandler> target, const std::shared_ptr<EventHandlerList> eventHandlerList)
				: Target(target),
				_eventHandlerList(eventHandlerList)
			{

			}
			Subscription(const Subscription&) = delete;

			Subscription(Subscription&& other) noexcept = default;

			virtual ~Subscription()
			{
				if (this->_eventHandlerList != nullptr)
					this->_eventHandlerList->remove(this->Target);
			}

		};

	private:
		class EventHandlerList
		{
		private:
			std::unordered_set<std::shared_ptr<EventHandler>> _handlers;

			std::optional<std::function<void(void)>> _subscriptionChanged;

			std::mutex _syncRoot;

		public:

			EventHandlerList(const EventHandlerList&) = delete;
			EventHandlerList(EventHandlerList&&) = delete;

			EventHandlerList()
				: _subscriptionChanged(std::nullopt)
			{
			}

			EventHandlerList(std::function<void(void)> subscriptionChanged)
				: _subscriptionChanged(subscriptionChanged)
			{
			}

			EventHandlerList(std::function<void(void)>&& subscriptionChanged)
				: _subscriptionChanged(subscriptionChanged)
			{
			}

			template <class TMethod, class TInstance>
			EventHandlerList(TMethod&& method, TInstance instance)
				: _subscriptionChanged(std::bind(method, instance))
			{
			}

			template <class TMethod>
			EventHandlerList(TMethod&& method)
				: _subscriptionChanged(std::bind(method))
			{
			}

			virtual ~EventHandlerList() noexcept = default;

			[[nodiscard]]
			std::shared_ptr<EventHandler> add(EventHandler eventHandler)
			{
				auto pEventListener = std::make_shared<EventHandler>(eventHandler);

				const std::lock_guard<std::mutex> lock(this->_syncRoot);

				this->_handlers.insert(pEventListener);

				if (this->_subscriptionChanged.has_value())
					this->_subscriptionChanged.value()();

				return pEventListener;
			}

			void remove(std::shared_ptr<EventHandler> eventHandler)
			{
				const std::lock_guard<std::mutex> lock(this->_syncRoot);

				this->_handlers.erase(eventHandler);

				if (this->_subscriptionChanged.has_value())
					this->_subscriptionChanged.value()();
			}

			void operator()(TArgs& args)
			{
				const std::lock_guard<std::mutex> lock(this->_syncRoot);

				for (const std::shared_ptr<EventHandler> handler : this->_handlers)
					handler->operator()(args);
			}

			void operator()(TArgs&& args)
			{
				const std::lock_guard<std::mutex> lock(this->_syncRoot);

				for (const std::shared_ptr<EventHandler> handler : this->_handlers)
					handler->operator()(args);
			}

			[[nodiscard]]
			constexpr bool empty() const
			{
				return this->_handlers.empty();
			}
		};

#pragma endregion

	private:
		const std::shared_ptr<EventHandlerList> _eventHandlerList;

	public:
		Event() noexcept
			: _eventHandlerList(std::make_shared<EventHandlerList>())
		{
		}

		Event(std::function<void(void)> subscriptionChanged)
			: _eventHandlerList(std::make_shared<EventHandlerList>(subscriptionChanged))
		{
		}

		Event(std::function<void(void)>&& subscriptionChanged)
			: _eventHandlerList(std::make_shared<EventHandlerList>(subscriptionChanged))
		{
		}

		template <class TMethod>
		Event(TMethod method)
			: _eventHandlerList(std::make_shared<EventHandlerList>(method))
		{
		}

		template <class TMethod, class TInstance>
		Event(TMethod method, TInstance instance)
			: _eventHandlerList(std::make_shared<EventHandlerList>(method, instance))
		{
		}

		virtual ~Event() noexcept = default;

		[[nodiscard(NODISCARD_MSG_EVENTSUBSCRIPTION_DROPED)]]
		constexpr Subscription subscribe(EventHandler eventListener) const
		{
			std::shared_ptr<EventHandler> pEventListener = this->_eventHandlerList->add(eventListener);
		
			return Subscription(pEventListener, this->_eventHandlerList);
		}

		template <class TMethod, class TInstance>
		[[nodiscard(NODISCARD_MSG_EVENTSUBSCRIPTION_DROPED)]]
		constexpr Subscription subscribe(TMethod&& func, TInstance&& object) const
		{
			return this->subscribe(static_cast<EventHandler>(std::bind(func, object, std::placeholders::_1)));
		}

		template <class TMethod>
		[[nodiscard(NODISCARD_MSG_EVENTSUBSCRIPTION_DROPED)]]
		constexpr Subscription subscribe(TMethod&& func) const
		{
			return this->subscribe(static_cast<EventHandler>(std::bind(func, std::placeholders::_1)));
		}

	private:

		friend TEventSource;    //friend class

		constexpr void operator()(TArgs& args) const
		{
			this->_eventHandlerList->operator()(args);
		}

		constexpr void operator()(TArgs&& args) const
		{
			this->_eventHandlerList->operator()(args);
		}

		[[nodiscard]]
		constexpr bool has_subscribers() const
		{
			return !this->_eventHandlerList->empty();
		}
	};
}