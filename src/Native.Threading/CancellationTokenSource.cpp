#include "CancellationTokenSource.h"

namespace Native
{
	namespace Threading
	{
		CancellationTokenSource::CancellationTokenSource() noexcept
			: _waitHandle(std::make_shared<ManualResetEvent>(false ))
		{
		}

		void CancellationTokenSource::cancel() noexcept
		{
			this->_waitHandle->set();
		}

		CancellationToken CancellationTokenSource::token() const noexcept
		{
			return CancellationToken(this->_waitHandle);
		}
	}
}
