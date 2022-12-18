#pragma once

#include <chrono>

#include "CancellationToken.h"
#include "ManualResetEvent.h"

namespace Native
{
	namespace Threading
	{
		class CancellationTokenSource
		{
		private:

			const std::shared_ptr<ManualResetEvent> _cancellationWaitHandle;

		public:

			/// <summary>
			/// Creates a new CancellationTokenSource.
			/// </summary>
			CancellationTokenSource() noexcept;

			/// <summary>
			/// Creates a new CancellationTokenSource.
			/// </summary>
			/// <param name="delay">The time interval to wait before canceling this CancellationTokenSource.</param>
			CancellationTokenSource(const std::chrono::milliseconds delay) noexcept;

			/// <summary>
			/// Creates a new CancellationTokenSource.
			/// </summary>
			/// <param name="dueTime">The point in time at which this CancellationTokenSource should be canceled.</param>
			CancellationTokenSource(const std::chrono::system_clock::time_point dueTime) noexcept;

			CancellationTokenSource(const CancellationTokenSource&) = delete;
			CancellationTokenSource(CancellationTokenSource&&) noexcept = default;

			/// <summary>
			/// Communicates a request for cancellation.
			/// </summary>
			void cancel() noexcept;

			/// <summary>
			/// The CancellationToken associated with this CancellationTokenSource.
			/// </summary>
			const CancellationToken Token;
		};
	}
}

