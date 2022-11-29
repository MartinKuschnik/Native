#include "CancellationToken.h"

using namespace std::chrono_literals;

namespace Native
{
	namespace Threading
	{
		CancellationToken::CancellationToken() noexcept
			: _waitHandle(nullptr)
		{
		}

		CancellationToken::CancellationToken(const std::shared_ptr<const WaitHandle> waitHandle) noexcept
			: _waitHandle(waitHandle)
		{
		}

		const WaitHandle& CancellationToken::wait_handle() const noexcept
		{
			return *this->_waitHandle;
		}

		bool CancellationToken::is_cancellation_requested() const
		{
			return this->_waitHandle != nullptr && this->_waitHandle->wait_one(0ms);
		}
	}
}
