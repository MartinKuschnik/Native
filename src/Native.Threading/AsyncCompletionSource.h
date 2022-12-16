#pragma once

#include "AsyncCompletionWaitHandle.h"

namespace Native
{
	namespace Threading
	{
		template<typename T>
			requires std::is_default_constructible<T>::value
		class AsyncCompletionSource
		{
		private:
			const std::shared_ptr<AsyncCompletionWaitHandle<T>> _asyncCompletionWaitHandle;

		public:

			const std::shared_ptr<const AsyncCompletionWaitHandle<T>> WaitHandle;

			AsyncCompletionSource()
				: _asyncCompletionWaitHandle(std::make_shared<AsyncCompletionWaitHandle<T>>()),
				WaitHandle(_asyncCompletionWaitHandle)
			{

			}

			void complete(T value)
			{
				this->_asyncCompletionWaitHandle->set(value);
			}
		};
	}
}

