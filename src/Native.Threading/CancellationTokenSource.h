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

			std::shared_ptr<ManualResetEvent> _cancellationWaitHandle;

			CancellationTokenSource(const CancellationToken one) noexcept;

			CancellationTokenSource(const CancellationToken one, const CancellationToken two) noexcept;

			CancellationTokenSource(const CancellationToken one, const CancellationToken two, const CancellationToken three) noexcept;

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

			/// <summary>
			/// Creates a CancellationTokenSource that will be in the canceled state when the source token is in the canceled state.
			/// </summary>
			/// <param name="one">The first cancellation token to observe.</param>
			/// <returns>A CancellationTokenSource that is linked to the source token.</returns>
			static CancellationTokenSource CreateLinkedTokenSource(const CancellationToken one);

			/// <summary>
			/// Creates a CancellationTokenSource that will be in the canceled state when any of the source tokens are in the canceled state.
			/// </summary>
			/// <param name="one">The first cancellation token to observe.</param>
			/// <param name="two">The second cancellation token to observe.</param>
			/// <returns>A CancellationTokenSource that is linked to the source tokens.</returns>
			static CancellationTokenSource CreateLinkedTokenSource(const CancellationToken one, const CancellationToken two);

			/// <summary>
			/// Creates a CancellationTokenSource that will be in the canceled state when any of the source tokens are in the canceled state.
			/// </summary>
			/// <param name="one">The first cancellation token to observe.</param>
			/// <param name="two">The second cancellation token to observe.</param>
			/// <param name="three">The third cancellation token to observe.</param>
			/// <returns>A CancellationTokenSource that is linked to the source tokens.</returns>
			static CancellationTokenSource CreateLinkedTokenSource(const CancellationToken one, const CancellationToken two, const CancellationToken three);
		};
	}
}

