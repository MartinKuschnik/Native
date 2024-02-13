#pragma once


#include <execution>
#include <functional>
#include <optional>
#include <span>
#include <experimental/generator>

#include "ParallelOptions.h"
#include "Task.h"

#include <iostream>

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
				static void ForEach(std::experimental::generator<T>&& source, const ParallelOptions& parallel_options, std::function<void(T)> body)
				{
					// ToDo: Exception handling
					std::array<Task<void>, 64> tasks;

					std::span<Task<void>> used_tasks(tasks.begin(), parallel_options.MaxDegreeOfParallelism.value_or(tasks.size()));

					for (T element : source)
					{
						if (parallel_options.CancellationToken.is_cancellation_requested())
							return;

						std::optional<uint8_t> index = Task<>::WaitAny(used_tasks, parallel_options.CancellationToken);

						if (!index.has_value()) // no value if canceled
							return;

						Task<>& finished_task = tasks[index.value()];

						if (finished_task.failed())
							break; // exit the loop and let Task<>::WaitAll throw the exception

						tasks[index.value()] = Task<>::Run(&Parallel::Execute<T>, body, element);
					}

					if (parallel_options.CancellationToken.is_cancellation_requested())
						return;

					Task<>::WaitAll(used_tasks, parallel_options.CancellationToken);
				}

				template<typename T, typename TFunc>
					requires std::convertible_to<TFunc, std::function<void(T)>>
				static void ForEach(std::experimental::generator<T>&& source, const ParallelOptions& parallel_options, TFunc body)
				{
					ForEach(std::forward<std::experimental::generator<T>>(source), parallel_options, static_cast<std::function<void(T)>>(body));
				}

				template<typename T, typename TFunc, typename  TInstance>
				static void ForEach(std::experimental::generator<T>&& source, const ParallelOptions& parallel_options, TFunc&& body, TInstance&& args)
				{
					ForEach(std::forward<std::experimental::generator<T>>(source), parallel_options, std::bind(std::forward<TFunc>(body), std::forward<TInstance>(args), std::placeholders::_1));
				}

			private:

				template<typename T>
				static void Execute(std::function<void(T)> action_to_execute, T element)
				{
					action_to_execute(element);
				}
			};
		}
	}
}