#include "CancellationTokenSource.h"

#include "MultiWaitHandle.h"
#include "TimedResetEvent.h"

namespace Native
{
	namespace Threading
	{

		CancellationTokenSource::CancellationTokenSource(const CancellationToken one) noexcept
			: _cancellationWaitHandle(std::make_shared<ManualResetEvent>(false)),
			Token(CancellationToken(std::make_shared<MultiWaitHandle>(_cancellationWaitHandle, one.WaitHandle)))
		{

		}

		CancellationTokenSource::CancellationTokenSource(const CancellationToken one, const CancellationToken two) noexcept
			: _cancellationWaitHandle(std::make_shared<ManualResetEvent>(false)),
			Token(CancellationToken(std::make_shared<MultiWaitHandle>(_cancellationWaitHandle, one.WaitHandle, two.WaitHandle)))
		{

		}

		CancellationTokenSource::CancellationTokenSource(const CancellationToken one, const CancellationToken two, const CancellationToken three) noexcept
			: _cancellationWaitHandle(std::make_shared<ManualResetEvent>(false)),
			Token(CancellationToken(std::make_shared<MultiWaitHandle>(_cancellationWaitHandle, one.WaitHandle, two.WaitHandle, three.WaitHandle)))
		{

		}

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

		CancellationTokenSource CancellationTokenSource::CreateLinkedTokenSource(const CancellationToken one)
		{
			return CancellationTokenSource(one);
		}

		CancellationTokenSource CancellationTokenSource::CreateLinkedTokenSource(const CancellationToken one, const CancellationToken two)
		{
			return CancellationTokenSource(one, two);
		}

		CancellationTokenSource CancellationTokenSource::CreateLinkedTokenSource(const CancellationToken one, const CancellationToken two, const CancellationToken three)
		{
			return CancellationTokenSource(one, two, three);
		}
	}
}
