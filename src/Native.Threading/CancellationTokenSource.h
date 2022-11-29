#pragma once

#include "CancellationToken.h"
#include "ManualResetEvent.h"

namespace Native
{
	namespace Threading
	{
		class CancellationTokenSource
		{
		public: 

			CancellationTokenSource() noexcept;
			CancellationTokenSource(const CancellationTokenSource&) = delete;
			CancellationTokenSource(CancellationTokenSource&&) noexcept = default;

			void cancel() noexcept;

			CancellationToken token() const noexcept;

		private:

			const std::shared_ptr<ManualResetEvent> _waitHandle;
		};
	}
}

