#pragma once


#include <execution>
#include <experimental/generator>
#include <functional>
#include <span>

#include "ParallelOptions.h"
#include "Task.h"


namespace Native
{
	namespace Threading
	{
		namespace Tasks
		{
			class Parallel
			{
			public: 
				// static class
				Parallel() = delete;

				template<typename T>
				constexpr static void ForEach(std::experimental::generator<T>&& source, const ParallelOptions& parallel_options, std::function<void(T)> body)
				{
					InternalForEach<T>(std::move(source), parallel_options, body);
				}

				template<typename T>
				constexpr static void ForEach(const std::span<T>& source, const ParallelOptions& parallel_options, std::function<void(T)> body)
				{
					InternalForEach<T>(source, parallel_options, body);
				}

				template<typename T, typename TFunc>
					requires std::convertible_to<TFunc, std::function<void(T)>>
				constexpr static void ForEach(std::experimental::generator<T>&& source, const ParallelOptions& parallel_options, TFunc&& func)
				{
					InternalForEach<T>(std::move(source), parallel_options, std::forward<std::function<void(T)>>(func));
				}

				template<typename T, typename TFunc>
					requires std::convertible_to<TFunc, std::function<void(T)>>
				constexpr static void ForEach(const std::span<T>& source, const ParallelOptions& parallel_options, TFunc&& func)
				{
					InternalForEach<T>(source, parallel_options, std::forward<std::function<void(T)>>(func));
				}

				template<typename T, typename TFunc, typename  TInstance>
				constexpr static void ForEach(std::experimental::generator<T>&& source, const ParallelOptions& parallel_options, TFunc&& func, TInstance&& instance)
				{
					InternalForEach<T>(std::move(source), parallel_options, std::bind(std::forward<TFunc>(func), std::forward<TInstance>(instance), std::placeholders::_1));
				}

				template<typename T, typename TFunc, typename  TInstance>
				constexpr static void ForEach(const std::span<T>& source, const ParallelOptions& parallel_options, TFunc&& func, TInstance&& instance)
				{
					InternalForEach<T>(source, parallel_options, std::bind(std::forward<TFunc>(func), std::forward<TInstance>(instance), std::placeholders::_1));
				}

			private:

				template<typename TElement, typename TElementSource>
				static void InternalForEach(TElementSource&& source, const ParallelOptions& parallel_options, std::function<void(TElement)> body)
				{
					std::array<Task<void>, 64> tasks;

					std::span<Task<void>> used_tasks(tasks.begin(), parallel_options.MaxDegreeOfParallelism.value_or(tasks.size()));

					for (TElement element : source)
					{
						if (parallel_options.CancellationToken.is_cancellation_requested())
							return;

						std::optional<uint8_t> index = Task<>::WaitAny(used_tasks, parallel_options.CancellationToken);

						if (!index.has_value()) // no value if canceled
							return;

						Task<>& finished_task = tasks[index.value()];

						if (finished_task.failed())
							break; // exit the loop and let Task<>::WaitAll throw the exception

						tasks[index.value()] = Task<>::Run(&Parallel::Execute<TElement>, body, element);
					}

					if (parallel_options.CancellationToken.is_cancellation_requested())
						return;

					Task<>::WaitAll(used_tasks, parallel_options.CancellationToken);
				}

				template<typename T>
				static void Execute(std::function<void(T)> action_to_execute, T param)
				{
					action_to_execute(param);
				}
			};
		}
	}
}