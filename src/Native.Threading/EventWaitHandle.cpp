#include "EventWaitHandle.h"

#include "ArgumentException.h"

#include "Win32Exception.h"

using namespace std::chrono_literals;

namespace Native
{
	namespace Threading
	{
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
		{
			this->_handle = other._handle;
			other._handle = INVALID_HANDLE_VALUE;
		}

		EventWaitHandle::~EventWaitHandle() noexcept
		{
			if (this->_handle != INVALID_HANDLE_VALUE)
				CloseHandle(this->_handle);
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

		bool EventWaitHandle::wait_one(const std::chrono::milliseconds timeout) const
		{
			DWORD result = WaitForSingleObject(this->_handle, static_cast<DWORD>(timeout.count()));
			
			switch (result)
			{
				case WAIT_OBJECT_0:
					return true;
				case WAIT_TIMEOUT:
					return false;
				default:
					throw Windows::Win32Exception(GetLastError(), nameof(WaitForSingleObject));
			}
		}

		bool EventWaitHandle::wait_one() const
		{
			return this->wait_one(-1s);
		}

		HANDLE EventWaitHandle::handle() const
		{
			return this->_handle;
		}
	}
}
