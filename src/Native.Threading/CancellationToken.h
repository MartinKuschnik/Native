#pragma once

#include <memory>

#include "WaitHandle.h"

namespace Native
{
	namespace Threading
	{
		class CancellationToken
		{
		public:

			CancellationToken() noexcept;

			CancellationToken(const std::shared_ptr<const WaitHandle> waitHandle) noexcept;

			const WaitHandle& wait_handle() const noexcept;

			bool is_cancellation_requested() const;

		private:

			const std::shared_ptr<const WaitHandle> _waitHandle;
		};
	}
}

