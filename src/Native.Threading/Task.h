#pragma once

#include <functional>
#include <future>
#include <ranges>

#include <AggregateException.h>
#include <ManualResetEvent.h>
#include <MultiWaitHandle.h>
#include <WaitHandle.h>

namespace Native
{
	namespace Threading
	{
		namespace Tasks
		{
			template<class TResult = void>
			class Task
			{
			private:

				template <typename TResult>
				struct Callable
				{
					virtual TResult invoke() const = 0;

					virtual bool failed() const = 0;
				};

				template <typename TResult, typename TFunc, typename ... TArgs>
				struct CallableWithStorage : public Callable<TResult>
				{
				private:
					TFunc _func;
					std::tuple<std::decay_t<TArgs>...> _args;
					mutable bool _failed = false;

				public:

					CallableWithStorage(TFunc&& func, TArgs&&... args) noexcept
						: _func(std::forward<TFunc>(func)),
						_args(std::forward<TArgs>(args)...)
					{
					}

					virtual TResult invoke() const override
					{
						try
						{
							return std::apply(this->_func, this->_args);
						}
						catch (...)
						{
							this->_failed = true;
							throw;
						}
					}

					virtual bool failed() const override
					{
						return this->_failed;
					}
				};

			private:

				std::shared_ptr<Callable<TResult>> _callable;

				std::shared_future<TResult> _future;

				std::shared_ptr<ManualResetEvent> _mutableWaitHandle;

				Task(std::shared_ptr<Callable<TResult>>&& callable)
					: _callable(std::move(callable)),
					_mutableWaitHandle(std::make_shared<ManualResetEvent>(false))

				{
				}

			public:

				Task() noexcept = default;

				Task(std::function<TResult()> action) noexcept
					: _callable(std::make_shared<CallableWithStorage<TResult, std::function<TResult()>>>(action)),
					_mutableWaitHandle(std::make_shared<ManualResetEvent>(false))
				{
				}

				void start()
				{
					if (this->_callable != nullptr && !this->_future.valid())
						this->_future = std::async(std::launch::async, &Task::RunAction, this->_callable, this->_mutableWaitHandle);
				}

				bool wait() const
				{
					if (this->_mutableWaitHandle == nullptr)
						return true;

					this->_mutableWaitHandle->wait_one();

					// to throw a cached exception
					this->_future.get();

					return true;
				}

				bool wait(const CancellationToken& cancellationToken) const
				{
					if (this->_mutableWaitHandle == nullptr)
						return true;

					if (!this->_mutableWaitHandle->wait_one(cancellationToken))
						return false;

					// to throw a cached exception
					this->_future.get();

					return true;
				}

				bool failed() const
				{
					return this->_callable != nullptr && this->_callable->failed();
				}

				TResult result() const
				{
					if constexpr (std::is_same_v<TResult, void>)
					{
						this->wait();
					}
					else
					{
						this->wait();

						return this->_future.get();
					}
				}

				template<typename TFunc, typename... ArgTypes>
				static Task Run(TFunc&& action, ArgTypes&&... args)
				{
					Task task(std::make_shared<CallableWithStorage<TResult, TFunc, ArgTypes...>>(std::forward<TFunc>(action), std::forward<ArgTypes>(args)...));

					task.start();

					return task;
				}

				/// <summary>
				/// Waits for any of the provided Task objects to complete execution.
				/// There is no exception thrown if one of the Task has failed with an exception.
				/// </summary>
				/// <param name="tasks">The Tasks on which to wait.</param>
				/// <returns>The index of the completed Task object in the tasks array.</returns>
				static uint8_t WaitAny(const std::span<const Task<TResult>> tasks)
				{
					std::vector<std::shared_ptr<WaitHandle>> wait_handles;

					wait_handles.reserve(tasks.size());

					for (const Task<TResult> task : tasks)
					{
						std::shared_ptr<WaitHandle> wait_handle = task._mutableWaitHandle;

						if (wait_handle == nullptr)
							return static_cast<uint8_t>(wait_handles.size());

						wait_handles.push_back(std::move(wait_handle));
					}

					return WaitHandle::WaitAny(wait_handles);
				}

				static uint8_t WaitAny(std::convertible_to<const Task<TResult>> auto&& ...tasks)
				{
					return WaitAny(std::initializer_list<const Task<TResult>>{tasks...});
				}

				/// <summary>
				/// Waits for any of the provided Task objects to complete execution.
				/// There is no exception thrown if one of the Task has failed with an exception.
				/// </summary>
				/// <param name="tasks">The Tasks on which to wait.</param>
				/// <param name="cancellationToken">The CancellationToken that can be used to cancel the wait.</param>
				/// <returns>The index of the completed Task object in the tasks array or no value if the operation was canceled.</returns>
				static std::optional<uint8_t> WaitAny(const std::span<const Task<TResult>> tasks, const CancellationToken& cancellationToken)
				{
					std::vector<std::shared_ptr<WaitHandle>> wait_handles;

					wait_handles.reserve(tasks.size());

					for (const Task<TResult> task : tasks)
					{
						std::shared_ptr<WaitHandle> wait_handle = task._mutableWaitHandle;

						if (wait_handle == nullptr)
							return static_cast<uint8_t>(wait_handles.size());

						wait_handles.push_back(std::move(wait_handle));
					}

					return WaitHandle::WaitAny(wait_handles, cancellationToken);
				}

				static uint8_t WaitAny(std::convertible_to<const Task<TResult>> auto&& ...tasks, const CancellationToken& cancellationToken)
				{
					return WaitAny(std::initializer_list<const Task<TResult>>{tasks...}, cancellationToken);
				}

				static bool WaitAll(const std::span<const Task<TResult>> tasks)
				{
					return WaitAll(tasks, CancellationToken::None);
				}

				static bool WaitAll(const std::span<const Task<TResult>> tasks, const CancellationToken& cancellationToken)
				{
					std::vector<std::shared_ptr<WaitHandle>> wait_handles;

					wait_handles.reserve(tasks.size());

					for (const Task<TResult> task : tasks)
					{
						std::shared_ptr<WaitHandle> wait_handle = task._mutableWaitHandle;

						if (wait_handle != nullptr)
							wait_handles.push_back(std::move(wait_handle));
					}

					const bool result = WaitHandle::WaitAll(wait_handles, cancellationToken);

					std::vector<std::exception_ptr> exceptions;

					for (const Task<TResult>& task : tasks)
					{
						try
						{
							// will re-throw the occurred exception
							task.result();
						}
						catch (...)
						{
							exceptions.push_back(std::current_exception());
						}
					}

					if (exceptions.empty())
						return result;

					throw Native::AggregateException(exceptions);
				}

			private:

				static TResult RunAction(std::shared_ptr<Callable<TResult>> callable, const std::shared_ptr<ManualResetEvent> wait_handle)
				{
					try
					{
						if constexpr (std::is_same_v<TResult, void>)
						{
							callable->invoke();

							wait_handle->set();
						}
						else
						{
							TResult result = callable->invoke();

							wait_handle->set();

							return result;
						}
					}
					catch (...)
					{
						wait_handle->set();

						throw;
					}
				}
			};
		}
	}
}