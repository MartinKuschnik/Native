#include "WaitHandle.h"

#include "CancellationToken.h"
#include "InvalidOperationException.h"
#include "Win32Exception.h"
#include "MultiWaitHandle.h"

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
				return WaitHandle::Wait(handels[0], timeout);
			else
				return WaitHandle::WaitAny(handels, handle_count, timeout).has_value();
		}

		bool WaitHandle::wait_one(const CancellationToken& cancellationToken) const
		{
			if (cancellationToken.WaitHandle == nullptr)
				return this->wait_one();

			HandleArray handels;

			const uint16_t handle_count_of_this = this->count_handles();
			const uint16_t handle_count_of_ct = cancellationToken.WaitHandle->count_handles();

			if (static_cast<size_t>(handle_count_of_this + handle_count_of_ct) > handels.size())
				throw InvalidOperationException("To much handles.");

			uint16_t handle_count = this->copy_handles(handels, 0);
			handle_count += cancellationToken.WaitHandle->copy_handles(handels, handle_count);

			// wait all handles
			WaitHandle::WaitAny(handels, handle_count, -1ms);

			// wait handles without the handles of the cancellation token to know if the cancellation token was canceled
			return WaitHandle::WaitAny(handels, handle_count_of_this, 0ms).has_value();
		}

		bool WaitHandle::WaitAll(const std::span<const std::shared_ptr<WaitHandle>> handles)
		{
			HandleArray _handels;

			uint16_t handle_count = 0;

			for (const std::shared_ptr<const WaitHandle>& handle : handles)
				handle_count += WaitHandle::CountHandles(handle);

			if (handle_count > _handels.size())
				throw InvalidOperationException("To much handles.");

			uint16_t copied = 0;

			for (const std::shared_ptr<const WaitHandle>& handle : handles)
				copied += WaitHandle::CopyHandles(handle, _handels, copied);

			if (handle_count == 0)
				return true;
			else if (handle_count == 1)
				return WaitHandle::Wait(_handels[0], -1s);
			else
				return WaitHandle::WaitAll(_handels, handle_count, -1s);
		}

		bool WaitHandle::WaitAll(const std::span<const std::shared_ptr<WaitHandle>> handles, const CancellationToken& cancellation_token)
		{
			for (const std::shared_ptr<const WaitHandle>& handle : handles)
			{
				if (!handle->wait_one(cancellation_token))
					return false;
			}

			return true;
		}

		uint8_t WaitHandle::WaitAny(const std::span<const std::shared_ptr<WaitHandle>> handles)
		{
			HandleArray _handels;

			uint16_t handle_count = 0;

			for (const std::shared_ptr<const WaitHandle>& handle : handles)
				handle_count += WaitHandle::CountHandles(handle);

			if (handle_count > _handels.size())
				throw InvalidOperationException("To much handles.");

			uint16_t copied = 0;

			for (const std::shared_ptr<const WaitHandle>& handle : handles)
				copied += WaitHandle::CopyHandles(handle, _handels, copied);

			if (handle_count == 0)
				throw InvalidOperationException("Waithandle span may not be empty. ");

			// wait all handles
			return WaitHandle::WaitAny(_handels, handle_count, -1ms).value();
		}

		std::optional<uint8_t> WaitHandle::WaitAny(const std::span<const std::shared_ptr<WaitHandle>> handles, const CancellationToken& cancellation_token)
		{
			if (cancellation_token.WaitHandle == nullptr)
				return WaitHandle::WaitAny(handles);

			HandleArray _handels;

			uint16_t handle_count = 0;

			for (const std::shared_ptr<const WaitHandle>& handle : handles)
				handle_count += WaitHandle::CountHandles(handle);

			if (handle_count == 0)
				throw InvalidOperationException("Waithandle span may not be empty. ");

			uint16_t cancellation_token_handle_count  = WaitHandle::CountHandles(cancellation_token.WaitHandle);

			const uint16_t total_handle_count = handle_count + cancellation_token_handle_count;

			if (total_handle_count > _handels.size())
				throw InvalidOperationException("To much handles.");

			uint16_t copied = 0;

			for (const std::shared_ptr<const WaitHandle>& handle : handles)
				copied += WaitHandle::CopyHandles(handle, _handels, copied);

			copied += WaitHandle::CopyHandles(cancellation_token.WaitHandle, _handels, copied);

			std::optional<uint8_t> index_satisfied = WaitHandle::WaitAny(_handels, total_handle_count, -1ms);

			if (index_satisfied > handle_count)
				return std::nullopt;

			return index_satisfied;
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

		bool WaitHandle::Wait(const HANDLE handle, const std::chrono::milliseconds timeout)
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

		std::optional<uint8_t> WaitHandle::WaitAny(const HandleArray& handles, const uint16_t handleCount, const std::chrono::milliseconds timeout)
		{
			DWORD result = WaitForMultipleObjects(handleCount, handles.data(), false, static_cast<DWORD>(timeout.count()));

			switch (result)
			{
			case WAIT_FAILED:
				throw Windows::Win32Exception(GetLastError(), nameof(WaitForMultipleObjects));
			case WAIT_TIMEOUT:
				return std::nullopt;
			default:
				return static_cast<uint8_t>(result);
			}
		}

		bool WaitHandle::WaitAll(const HandleArray& handles, const uint16_t handleCount, const std::chrono::milliseconds timeout)
		{
			DWORD result = WaitForMultipleObjects(handleCount, handles.data(), true, static_cast<DWORD>(timeout.count()));

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