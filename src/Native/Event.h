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
			const std::shared_ptr<Source> _source;

		public:
			const std::shared_ptr<std::function<void(TArgs&)>> Target;

			Subscription(const std::shared_ptr<std::function<void(TArgs&)>> target, const std::shared_ptr<Source> source)
				: Target(target),
				_source(source)
			{

			}
			Subscription(const Subscription&) = delete;

			Subscription(Subscription&& other) noexcept = default;

			virtual ~Subscription()
			{
				if (this->_source != nullptr)
					this->_source->unsubscribe(this->Target);
			}

		};

	private:
		class Source
		{
		private:
			std::unordered_set<std::shared_ptr<std::function<void(TArgs&)>>> _handlers;

			std::optional<std::function<void(void)>> _subscriptionChanged;

		public:

			Source(const Source&) = delete;
			Source(Source&&) = delete;

			Source()
				: _subscriptionChanged(std::nullopt)
			{
			}

			Source(std::function<void(void)> subscriptionChanged)
				: _subscriptionChanged(subscriptionChanged)
			{
			}

			Source(std::function<void(void)>&& subscriptionChanged)
				: _subscriptionChanged(subscriptionChanged)
			{
			}

			template <class TMethod, class TInstance>
			Source(TMethod&& method, TInstance instance)
				: _subscriptionChanged(std::bind(method, instance))
			{
			}

			template <class TMethod>
			Source(TMethod&& method)
				: _subscriptionChanged(std::bind(method))
			{
			}

			virtual ~Source() noexcept = default;

			[[nodiscard]]
			std::shared_ptr<std::function<void(TArgs&)>> subscribe(std::function<void(TArgs&)> eventListener)
			{
				auto pEventListener = std::make_shared<std::function<void(TArgs&)>>(eventListener);

				this->_handlers.insert(pEventListener);

				if (this->_subscriptionChanged.has_value())
					this->_subscriptionChanged.value()();

				return pEventListener;
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
					handler->operator()(args);
			}

			void operator()(TArgs&& args)
			{
				for (const std::shared_ptr<std::function<void(TArgs&)>> handler : this->_handlers)
					handler->operator()(args);
			}

			[[nodiscard]]
			constexpr bool has_subscribers() const
			{
				return !this->_handlers.empty();
			}
		};

#pragma endregion

	private:
		const std::shared_ptr<Source> _source;

	public:
		Event() noexcept
			: _source(std::make_shared<Source>())
		{
		}

		Event(std::function<void(void)> subscriptionChanged)
			: _source(std::make_shared<Source>(subscriptionChanged))
		{
		}

		Event(std::function<void(void)>&& subscriptionChanged)
			: _source(std::make_shared<Source>(subscriptionChanged))
		{
		}

		template <class TMethod>
		Event(TMethod method)
			: _source(std::make_shared<Source>(method))
		{
		}

		template <class TMethod, class TInstance>
		Event(TMethod method, TInstance instance)
			: _source(std::make_shared<Source>(method, instance))
		{
		}

		virtual ~Event() noexcept = default;

		[[nodiscard(NODISCARD_MSG_EVENTSUBSCRIPTION_DROPED)]]
		constexpr Subscription subscribe(std::function<void(TArgs&)> eventListener) const
		{
			std::shared_ptr<std::function<void(TArgs&)>> pEventListener = this->_source->subscribe(eventListener);
		
			return Subscription(pEventListener, this->_source);
		}

		template <class TMethod, class TInstance>
		[[nodiscard(NODISCARD_MSG_EVENTSUBSCRIPTION_DROPED)]]
		constexpr Subscription subscribe(TMethod&& func, TInstance&& object) const
		{
			return this->subscribe(static_cast<std::function<void(TArgs&)>>(std::bind(func, object, std::placeholders::_1)));
		}

		template <class TMethod>
		[[nodiscard(NODISCARD_MSG_EVENTSUBSCRIPTION_DROPED)]]
		constexpr Subscription subscribe(TMethod&& func) const
		{
			return this->subscribe(static_cast<std::function<void(TArgs&)>>(std::bind(func, std::placeholders::_1)));
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