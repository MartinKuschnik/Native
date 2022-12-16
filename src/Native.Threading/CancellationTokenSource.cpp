#include "CancellationTokenSource.h"

#include "MultiWaitHandle.h"
#include "TimedResetEvent.h"

namespace Native
{
	namespace Threading
	{
		CancellationTokenSource::CancellationTokenSource() noexcept
			: _cancellationWaitHandle(std::make_shared<ManualResetEvent>(false)),
			_allWaitHandles({ _cancellationWaitHandle })
		{
		}

		CancellationTokenSource::CancellationTokenSource(const std::chrono::milliseconds delay) noexcept
			: _cancellationWaitHandle(std::make_shared<ManualResetEvent>(false)),
			_allWaitHandles({ _cancellationWaitHandle, std::make_shared<TimedResetEvent>(delay) })
		{
		}

		void CancellationTokenSource::cancel() noexcept
		{
			this->_cancellationWaitHandle->set();
		}

		CancellationToken CancellationTokenSource::token() const noexcept
		{
			return CancellationToken(std::make_shared<MultiWaitHandle>(this->_allWaitHandles));
		}
	}
}
