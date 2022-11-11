#pragma once



#include <functional>
#include <memory>
#include <unordered_set>

#include <optional>

#include <Windows.h>

namespace Native
{
	template<class TArgs>
	class EventSource;

	template<class TArgs>
	class EventSubscription
	{
	private:
		EventSource<TArgs>* _source;

	public:
		const std::shared_ptr<std::function<void(TArgs&)>> Target;

		EventSubscription(const std::shared_ptr<std::function<void(TArgs&)>> target, EventSource<TArgs>* const source)
			: Target(target),
			_source(source)
		{

		}
		EventSubscription(const EventSubscription&) = delete;

		EventSubscription(EventSubscription&& other)
			: Target(std::move(other.Target)),
			_source(std::move(other._source))
		{
			other._source = nullptr;
		}

		virtual ~EventSubscription()
		{
			if (this->_source != nullptr)
				this->_source->unsubscribe(this->Target);
		}

	private:

	};


	template<class TArgs>
	class Event;

	template<class TArgs>
	class EventSource
	{
	private:
		std::unordered_set<std::shared_ptr<std::function<void(TArgs&)>>> _handlers;

		std::optional<std::function<void(void)>> _subscriptionChanged;

	public:
		EventSource()
			: _subscriptionChanged(std::nullopt)
		{
		}

		EventSource(std::function<void(void)> subscriptionChanged)
			: _subscriptionChanged(subscriptionChanged)
		{
		}

		EventSource(std::function<void(void)>&& subscriptionChanged)
			: _subscriptionChanged(subscriptionChanged)
		{
		}

		template <class TMethod, class TInstance>
		EventSource(TMethod&& method, TInstance instance)
			: _subscriptionChanged(std::bind(method, instance))
		{
		}

		template <class TMethod>
		EventSource(TMethod&& method)
			: _subscriptionChanged(std::bind(method))
		{
		}

		virtual ~EventSource() = default;

		[[nodiscard]] 
		EventSubscription<TArgs> subscribe(std::function<void(TArgs&)> eventListener)
		{
			auto pEventListener = std::make_shared<std::function<void(TArgs&)>>(eventListener);

			EventSubscription<TArgs> sub(pEventListener, this);

			this->_handlers.insert(pEventListener);

			if (this->_subscriptionChanged.has_value())
				this->_subscriptionChanged.value()();

			return sub;
		}

		void unsubscribe(std::shared_ptr<std::function<void(TArgs&)>> eventListener)
		{
			this->_handlers.erase(eventListener);

			if (this->_subscriptionChanged.has_value())
				this->_subscriptionChanged.value()();
		}

		void operator()(TArgs& args)
		{
			for (const std::shared_ptr<std::function<void(TArgs&)>> handler : this->_handlers)
			{
				handler->operator()(args);
			}
		}

		[[nodiscard]] 
		constexpr bool has_subscribers() const
		{
			return !this->_handlers.empty();
		}

		[[nodiscard]] 
		Event<TArgs> create_event()
		{
			return Event<TArgs>(this);
		}

	private:

	};

	template<class TArgs>
	class Event
	{
#define NODISCARD_MSG_EVENTSUBSCRIPTION_DROPED "This function creating a subscription represented by the return value. Discarding the return value will destroy the subscription instantly."

	private:
		EventSource<TArgs>* const _source;

	public:
		Event(EventSource<TArgs>* const source)
			: _source(source)
		{
		}

		virtual ~Event() = default;

		[[nodiscard(NODISCARD_MSG_EVENTSUBSCRIPTION_DROPED)]]
		EventSubscription<TArgs> subscribe(std::function<void(TArgs&)> eventListener) const
		{
			return this->_source->subscribe(eventListener);
		}

		template <class TMethod, class TInstance>
		[[nodiscard(NODISCARD_MSG_EVENTSUBSCRIPTION_DROPED)]]
		EventSubscription<TArgs> subscribe(TMethod&& func, TInstance&& object) const
		{
			return this->_source->subscribe(std::bind(func, object, std::placeholders::_1));
		}


		template <class TMethod>
		[[nodiscard(NODISCARD_MSG_EVENTSUBSCRIPTION_DROPED)]]
		EventSubscription<TArgs> subscribe(TMethod&& func) const
		{
			return this->_source->subscribe(std::bind(func, std::placeholders::_1));
		}
	};

	class ConsoleCancelEventArgs
	{
	private:
		bool _canceled = false;

	public:

		void Cancel() noexcept
		{
			this->_canceled = true;
		}

		constexpr bool is_canceled() const noexcept
		{
			return this->_canceled;
		}

	};

	class Console
	{
	private:

		static EventSource<ConsoleCancelEventArgs> _CancelKeyPressEventSource;

		static bool _CtrlHandlerSetted;

		Console(); // static class

		static void CancelKeyPressEventChanged();

		static BOOL ConsoleCtrlHandlerRoutine(DWORD dwCtrlType) noexcept;

	public:

		static const Event<ConsoleCancelEventArgs> CancelKeyPress;

	};
}