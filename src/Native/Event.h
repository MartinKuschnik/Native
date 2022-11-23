#pragma once

#define NODISCARD_MSG_EVENTSUBSCRIPTION_DROPED "This function creating a subscription represented by the return value. Discarding the return value will destroy the subscription instantly."

#include <functional>
#include <memory>
#include <optional>
#include <unordered_set>

namespace Native
{
	template<class TArgs, class TEventSource>
	class Event
	{

#pragma region Subclasses

	private:
		class Source;

	public:
		class Subscription
		{
		private:
			Source* _source;

			const std::shared_ptr<std::atomic_bool> _signal;

		public:
			const std::shared_ptr<std::function<void(TArgs&)>> Target;

			Subscription(const std::shared_ptr<std::function<void(TArgs&)>> target, Source* const source, const std::shared_ptr<std::atomic_bool> signal)
				: Target(target),
				_source(source),
				_signal(signal)
			{

			}
			Subscription(const Subscription&) = delete;

			Subscription(Subscription&& other) noexcept
				: Target(other.Target),
				_source(std::move(other._source)),
				_signal(other._signal)
			{
				other._source = nullptr;
			}

			virtual ~Subscription()
			{
				if (this->_source != nullptr && *this->_signal == true)
					this->_source->unsubscribe(this->Target);
			}

		private:

		};

	private:
		class Source
		{
		private:
			std::unordered_set<std::shared_ptr<std::function<void(TArgs&)>>> _handlers;

			std::optional<std::function<void(void)>> _subscriptionChanged;

			std::shared_ptr<std::atomic_bool> _subscriptionSignal;

		public:

			Source(const Source&) = delete;
			Source(Source&&) = delete;

			Source()
				: _subscriptionChanged(std::nullopt),
				_subscriptionSignal(std::make_shared<std::atomic_bool>(true))
			{
			}

			Source(std::function<void(void)> subscriptionChanged)
				: _subscriptionChanged(subscriptionChanged),
				_subscriptionSignal(std::make_shared<std::atomic_bool>(true))
			{
			}

			Source(std::function<void(void)>&& subscriptionChanged)
				: _subscriptionChanged(subscriptionChanged),
				_subscriptionSignal(std::make_shared<std::atomic_bool>(true))
			{
			}

			template <class TMethod, class TInstance>
			Source(TMethod&& method, TInstance instance)
				: _subscriptionChanged(std::bind(method, instance)),
				_subscriptionSignal(std::make_shared<std::atomic_bool>(true))
			{
			}

			template <class TMethod>
			Source(TMethod&& method)
				: _subscriptionChanged(std::bind(method)),
				_subscriptionSignal(std::make_shared<std::atomic_bool>(true))
			{
			}

			virtual ~Source()
			{
				*this->_subscriptionSignal = false;
			}

			[[nodiscard]]
			Subscription subscribe(std::function<void(TArgs&)> eventListener)
			{
				auto pEventListener = std::make_shared<std::function<void(TArgs&)>>(eventListener);

				Subscription sub(pEventListener, this, this->_subscriptionSignal);

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

			void operator()(TArgs&& args)
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
		};

#pragma endregion

	private:
		const std::unique_ptr<Source> _source;

	public:
		Event() noexcept
			: _source(std::make_unique<Source>())
		{
		}

		Event(std::function<void(void)> subscriptionChanged)
			: _source(std::make_unique<Source>(subscriptionChanged))
		{
		}

		Event(std::function<void(void)>&& subscriptionChanged)
			: _source(std::make_unique<Source>(subscriptionChanged))
		{
		}

		template <class TMethod>
		Event(TMethod method)
			: _source(std::make_unique<Source>(method))
		{
		}

		template <class TMethod, class TInstance>
		Event(TMethod method, TInstance instance)
			: _source(std::make_unique<Source>(method, instance))
		{
		}

		virtual ~Event() noexcept = default;

		[[nodiscard(NODISCARD_MSG_EVENTSUBSCRIPTION_DROPED)]]
		constexpr Subscription subscribe(std::function<void(TArgs&)> eventListener) const
		{
			return this->_source->subscribe(eventListener);
		}

		template <class TMethod, class TInstance>
		[[nodiscard(NODISCARD_MSG_EVENTSUBSCRIPTION_DROPED)]]
		constexpr Subscription subscribe(TMethod&& func, TInstance&& object) const
		{
			return this->_source->subscribe(std::bind(func, object, std::placeholders::_1));
		}

		template <class TMethod>
		[[nodiscard(NODISCARD_MSG_EVENTSUBSCRIPTION_DROPED)]]
		constexpr Subscription subscribe(TMethod&& func) const
		{
			return this->_source->subscribe(std::bind(func, std::placeholders::_1));
		}

	private:

		friend TEventSource;    //friend class

		constexpr void operator()(TArgs& args) const
		{
			this->_source->operator()(args);
		}

		constexpr void operator()(TArgs&& args) const
		{
			this->_source->operator()(args);
		}

		[[nodiscard]]
		constexpr bool has_subscribers() const
		{
			return this->_source->has_subscribers();
		}
	};
}