#include "CancellationToken.h"

#include "WaitHandle.h"

using namespace std::chrono_literals;

namespace Native
{
	namespace Threading
	{
		const CancellationToken CancellationToken::None = CancellationToken();

		CancellationToken::CancellationToken(const std::shared_ptr<const Threading::WaitHandle> waitHandle) noexcept
			: WaitHandle(waitHandle)
		{
		}

		bool CancellationToken::is_cancellation_requested() const
		{
			return this->WaitHandle != nullptr && this->WaitHandle->wait_one(0ms);
		}
	}
}
