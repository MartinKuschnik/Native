#include "EventWaitHandle.h"

#include "ArgumentException.h"

#include "Win32Exception.h"

using namespace std::chrono_literals;

namespace Native
{
	namespace Threading
	{
		EventWaitHandle::EventWaitHandle(Windows::Handle&& handle) noexcept
			: _handle(std::move(handle))
		{
		}

		EventWaitHandle::EventWaitHandle(bool initialState, EventResetMode mode) noexcept
			: EventWaitHandle(initialState, mode, std::string_view())
		{

		}

		EventWaitHandle::EventWaitHandle(bool initialState, EventResetMode mode, std::string_view name)
		{
			if (MAX_PATH < name.length())
				throw ArgumentException("Wait handle name too long");

			this->_handle = CreateEventA(nullptr, mode == EventResetMode::ManualReset, initialState, name.data());

			if (this->_handle == NULL)
				throw Windows::Win32Exception(GetLastError(), nameof(CreateEventA));
		}

		EventWaitHandle::EventWaitHandle(bool initialState, EventResetMode mode, std::wstring_view name)
		{
			if (MAX_PATH < name.length())
				throw ArgumentException("Wait handle name too long");

			this->_handle = CreateEventW(nullptr, mode == EventResetMode::ManualReset, initialState, name.data());

			if (this->_handle == NULL)
				throw Windows::Win32Exception(GetLastError(), nameof(CreateEventW));
		}

		EventWaitHandle::EventWaitHandle(EventWaitHandle&& other) noexcept
			: _handle(std::move(other._handle))
		{
		}

		void EventWaitHandle::reset()
		{
			BOOL ret = ResetEvent(this->_handle);

			if (!ret)
				throw Windows::Win32Exception(GetLastError(), nameof(ResetEvent));
		}

		void EventWaitHandle::set()
		{
			BOOL ret = SetEvent(this->_handle);

			if (!ret)
				throw Windows::Win32Exception(GetLastError(), nameof(SetEvent));
		}

		std::optional<EventWaitHandle> EventWaitHandle::OpenExisting(std::string_view name)
		{
			if (MAX_PATH < name.length())
				throw ArgumentException("Wait handle name too long");

			constexpr DWORD access_rights = MAXIMUM_ALLOWED | SYNCHRONIZE | EVENT_MODIFY_STATE;

			Windows::Handle handle = OpenEventA(access_rights, false, name.data());

			if (handle == NULL)
			{
				const DWORD error = GetLastError();

				if (error == ERROR_FILE_NOT_FOUND)
					return std::nullopt;

				throw Windows::Win32Exception(error, nameof(CreateEventW));
			}			

			return EventWaitHandle(std::move(handle));
		}

		uint16_t EventWaitHandle::count_handles() const
		{
			return 1;
		}

		uint16_t EventWaitHandle::copy_handles(HandleArray& dest, const uint16_t index) const
		{
			dest[index] = this->_handle;

			return 1;
		}
	}
}
