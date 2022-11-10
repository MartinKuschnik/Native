#pragma once

#include <functional>
#include <memory>
#include <unordered_set>

namespace Native
{
	template<class TArgs>
	class EventSource;

	template<class TArgs>
	class EventRegistration
	{
	private:
		EventSource<TArgs>* _source;

	public:
		const std::shared_ptr<std::function<void(TArgs)>> Target;

		EventRegistration(const std::shared_ptr<std::function<void(TArgs)>> target, EventSource<TArgs>* const source)
			: Target(target),
			_source(source)
		{

		}
		EventRegistration(const EventRegistration&) = delete;

		EventRegistration(EventRegistration&& other)
			: Target(std::move(other.Target)),
			_source(std::move(other._source))
		{
			other._source = nullptr;
		}

		virtual ~EventRegistration()
		{
			if (this->_source != nullptr)
				this->_source->remove_handler(this->Target);
		}

	private:

	};


	template<class TArgs>
	class Event;

	template<class TArgs>
	class EventSource
	{
	private:
		std::unordered_set<std::shared_ptr<std::function<void(TArgs)>>> _handlers;

	public:
		EventSource() = default;
		virtual ~EventSource() = default;

		EventRegistration<TArgs> add_handler(std::function<void(TArgs)> eventListener)
		{
			auto pEventListener = std::make_shared<std::function<void(TArgs)>>(eventListener);

			EventRegistration<TArgs> reg(pEventListener, this);

			this->_handlers.insert(pEventListener);

			return reg;
		}


		void remove_handler(std::shared_ptr<std::function<void(TArgs)>> eventListener)
		{
			this->_handlers.erase(eventListener);
		}

		void operator () (TArgs args)
		{
			for (const std::shared_ptr<std::function<void(TArgs)>> handler : this->_handlers)
			{
				handler->operator()(args);
			}
		}

		Event<TArgs> create_event() 
		{
			return Event<TArgs>(this);
		}

	private:

	};

	template<class TArgs>
	class Event
	{
	private:
		EventSource<TArgs>* const _source;

	public:
		Event(EventSource<TArgs>* const source)
			: _source(source)
		{
		}

		virtual ~Event() = default;

		EventRegistration<TArgs> operator +=(std::function<void(TArgs)> eventListener) const
		{
			return this->_source->add_handler(eventListener);
		}
	};

	class ConsoleCancelEventArgs
	{
	public:
		int Al;

		ConsoleCancelEventArgs() = default;
		ConsoleCancelEventArgs(int a)
			: Al(a)
		{

		}
		~ConsoleCancelEventArgs() = default;

	private:

	};

	class Console
	{
	private:

		Console(); // static class

		static EventSource<ConsoleCancelEventArgs> CancelKeyPressEventSource;

	public:

		static const Event<ConsoleCancelEventArgs> CancelKeyPress;


	};
}