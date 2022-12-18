#include "MultiWaitHandle.h"
namespace Native
{
	namespace Threading
	{
		MultiWaitHandle::MultiWaitHandle(const std::shared_ptr<WaitHandle> waitHandle1, const std::shared_ptr<WaitHandle> waitHandle2)
			:_handles({ waitHandle1, waitHandle2 })
		{
		}

		MultiWaitHandle::MultiWaitHandle(const std::shared_ptr<WaitHandle> waitHandle1, const std::shared_ptr<WaitHandle> waitHandle2, const std::shared_ptr<WaitHandle> waitHandle3)
			:_handles({ waitHandle1, waitHandle2 })
		{
		}

		MultiWaitHandle::MultiWaitHandle(const std::shared_ptr<WaitHandle> waitHandle1, const std::shared_ptr<WaitHandle> waitHandle2, const std::shared_ptr<WaitHandle> waitHandle3, const std::shared_ptr<WaitHandle> waitHandle4)
			:_handles({ waitHandle1, waitHandle2 })
		{
		}

		MultiWaitHandle::MultiWaitHandle(const std::shared_ptr<WaitHandle> waitHandle1, const std::shared_ptr<WaitHandle> waitHandle2, const std::shared_ptr<WaitHandle> waitHandle3, const std::shared_ptr<WaitHandle> waitHandle4, const std::shared_ptr<WaitHandle> waitHandle5)
			:_handles({ waitHandle1, waitHandle2 })
		{
		}

		uint16_t MultiWaitHandle::count_handles() const
		{
			uint16_t count = 0;

			for (const std::shared_ptr<WaitHandle>& handle : this->_handles)
				count += WaitHandle::CountHandles(handle);

			return count;
		}

		uint16_t MultiWaitHandle::copy_handles(HandleArray& dest, uint16_t index) const
		{
			uint16_t copied = 0;

			for (const std::shared_ptr<WaitHandle>& handle : this->_handles)
				copied += WaitHandle::CopyHandles(handle, dest, index + copied);

			return copied;
		}
	}
}
