#include "CancellationTokenSource.h"

#include "MultiWaitHandle.h"
#include "TimedResetEvent.h"

namespace Native
{
	namespace Threading
	{
		CancellationTokenSource::CancellationTokenSource() noexcept
			: _cancellationWaitHandle(std::make_shared<ManualResetEvent>(false)),
			Token(CancellationToken(_cancellationWaitHandle ))
		{
		}

		CancellationTokenSource::CancellationTokenSource(const std::chrono::milliseconds delay) noexcept
			: _cancellationWaitHandle(std::make_shared<ManualResetEvent>(false)),
			Token(CancellationToken(std::make_shared<MultiWaitHandle>( _cancellationWaitHandle, std::make_shared<TimedResetEvent>(delay))))
		{
		}

		CancellationTokenSource::CancellationTokenSource(const std::chrono::system_clock::time_point dueTime) noexcept
			: _cancellationWaitHandle(std::make_shared<ManualResetEvent>(false)),
			Token(CancellationToken(std::make_shared<MultiWaitHandle>( _cancellationWaitHandle, std::make_shared<TimedResetEvent>(dueTime))))
		{
		}

		void CancellationTokenSource::cancel() noexcept
		{
			this->_cancellationWaitHandle->set();
		}
	}
}
