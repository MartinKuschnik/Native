#include "WaitHandle.h"

#include "CancellationToken.h"
#include "InvalidOperationException.h"
#include "Win32Exception.h"

using namespace std::chrono_literals;

namespace Native
{
	namespace Threading
	{
		bool WaitHandle::wait_one(const std::chrono::milliseconds timeout) const
		{
			HandleArray handels;

			if (this->count_handles() > handels.size())
				throw InvalidOperationException("To much handles.");

			const uint16_t handle_count = this->copy_handles(handels, 0);

			if (handle_count == 0)
				return true;
			else if (handle_count == 1)
				return WaitHandle::WaitOne(handels[0], timeout);
			else
				return WaitHandle::WaitOne(handels, handle_count, timeout);
		}

		bool WaitHandle::wait_one(const CancellationToken& cancellationToken) const
		{
			HandleArray handels;

			const uint16_t handle_count_of_this = this->count_handles();
			const uint16_t handle_count_of_ct = cancellationToken.WaitHandle->count_handles();

			if (static_cast<size_t>(handle_count_of_this + handle_count_of_ct) > handels.size())
				throw InvalidOperationException("To much handles.");

			uint16_t handle_count = this->copy_handles(handels, 0);
			handle_count += cancellationToken.WaitHandle->copy_handles(handels, handle_count);

			WaitHandle::WaitOne(handels, handle_count, -1ms);

			return WaitHandle::WaitOne(handels, handle_count_of_this, 0ms);
		}

		bool WaitHandle::wait_one() const
		{
			return this->wait_one(-1ms);
		}

		uint16_t WaitHandle::CountHandles(const WaitHandle& handle)
		{
			return handle.count_handles();
		}

		uint16_t WaitHandle::CountHandles(const std::shared_ptr<const WaitHandle>& handle)
		{
			return handle->count_handles();
		}

		uint16_t WaitHandle::CopyHandles(const WaitHandle& handle, HandleArray& dest, const uint16_t index)
		{
			return handle.copy_handles(dest, index);
		}

		uint16_t WaitHandle::CopyHandles(const std::shared_ptr<const WaitHandle>& handle, HandleArray& dest, const uint16_t index)
		{
			return handle->copy_handles(dest, index);
		}

		bool WaitHandle::WaitOne(const HANDLE handle, const std::chrono::milliseconds timeout)
		{
			DWORD result = WaitForSingleObject(handle, static_cast<DWORD>(timeout.count()));

			switch (result)
			{
				case WAIT_FAILED:
					throw Windows::Win32Exception(GetLastError(), nameof(WaitForSingleObject));
				case WAIT_TIMEOUT:
					return false;
				default:
					return true;
			}
		}

		bool WaitHandle::WaitOne(const HandleArray& handles, const uint16_t handleCount, const std::chrono::milliseconds timeout)
		{
			DWORD result = WaitForMultipleObjects(handleCount, handles.data(), false, static_cast<DWORD>(timeout.count()));

			switch (result)
			{
				case WAIT_FAILED:
					throw Windows::Win32Exception(GetLastError(), nameof(WaitForMultipleObjects));
				case WAIT_TIMEOUT:
					return false;
				default:
					return true;
			}
		}
	}
}