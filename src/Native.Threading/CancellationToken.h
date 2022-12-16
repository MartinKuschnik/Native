#pragma once

#include <memory>

namespace Native
{
	namespace Threading
	{
		class WaitHandle; // use forward deflation because the WaitHandle includes this header

		class CancellationToken
		{
		public:

			CancellationToken() noexcept;

			CancellationToken(const std::shared_ptr<const WaitHandle> waitHandle) noexcept;

			bool is_cancellation_requested() const;

			const std::shared_ptr<const WaitHandle> WaitHandle;
		private:

		};
	}
}

