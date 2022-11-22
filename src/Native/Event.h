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

	public:

#pragma region Subclasses

		class Source;

		class Subscription
		{
		private:
			Source* _source;

		public:
			const std::shared_ptr<std::function<void(TArgs&)>> Target;

			Subscription(const std::shared_ptr<std::function<void(TArgs&)>> target, Source* const source)
				: Target(target),
				_source(source)
			{

			}
			Subscription(const Subscription&) = delete;

			Subscription(Subscription&& other) noexcept
				: Target(std::move(other.Target)),
				_source(std::move(other._source))
			{
				other._source = nullptr;
			}

			virtual ~Subscription()
			{
				if (this->_source != nullptr)
					this->_source->unsubscribe(this->Target);
			}

		private:

		};

		class Source
		{
		private:
			std::unordered_set<std::shared_ptr<std::function<void(TArgs&)>>> _handlers;

			std::optional<std::function<void(void)>> _subscriptionChanged;

		public:
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

			virtual ~Source() = default;

			[[nodiscard]]
			Subscription subscribe(std::function<void(TArgs&)> eventListener)
			{
				auto pEventListener = std::make_shared<std::function<void(TArgs&)>>(eventListener);

				Subscription sub(pEventListener, this);

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


		private:

		};

#pragma endregion

	private:
		const std::unique_ptr<Source> _source;
		
		friend TEventSource;    //friend class

		constexpr void operator ()(TArgs& args) const
		{
			this->_source->operator()(args);
		}

		constexpr void operator ()(TArgs&& args) const
		{
			this->_source->operator()(args);
		}

		[[nodiscard]]
		constexpr bool has_subscribers() const
		{
			return this->_source->has_subscribers();
		}

	public:
		Event() noexcept
			: _source(std::make_unique<Source>())
		{
		}

		virtual ~Event() noexcept = default;

		void operator = (const Event& other) noexcept
		{
			this->_source = other._source;
		}

		void operator = (Event&& other) noexcept
		{
			this->_source = other._source;
		}

		[[nodiscard(NODISCARD_MSG_EVENTSUBSCRIPTION_DROPED)]]
		Subscription subscribe(std::function<void(TArgs&)> eventListener) const
		{
			return this->_source->subscribe(eventListener);
		}

		template <class TMethod, class TInstance>
		[[nodiscard(NODISCARD_MSG_EVENTSUBSCRIPTION_DROPED)]]
		Subscription subscribe(TMethod&& func, TInstance&& object) const
		{
			return this->_source->subscribe(std::bind(func, object, std::placeholders::_1));
		}

		template <class TMethod>
		[[nodiscard(NODISCARD_MSG_EVENTSUBSCRIPTION_DROPED)]]
		Subscription subscribe(TMethod&& func) const
		{
			return this->_source->subscribe(std::bind(func, std::placeholders::_1));
		}
	};
}