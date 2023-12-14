#pragma once

#include <memory>

#include <WaitHandle.h>

namespace Native
{
	namespace Threading
	{
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

