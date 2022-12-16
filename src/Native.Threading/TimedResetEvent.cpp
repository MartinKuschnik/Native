#include "TimedResetEvent.h"

#include "ArgumentException.h"
#include "Win32Exception.h"

namespace Native
{
	namespace Threading
	{

		TimedResetEvent::TimedResetEvent(const std::chrono::milliseconds delay) noexcept
			: TimedResetEvent(delay, std::string_view())
		{
		}

		TimedResetEvent::TimedResetEvent(const std::chrono::milliseconds delay, std::string_view name)
		{
			if (MAX_PATH < name.length())
				throw ArgumentException("Wait handle name too long");

			this->_handle = CreateWaitableTimerA(nullptr, true, name.data());

			if (this->_handle == NULL)
				throw Windows::Win32Exception(GetLastError(), nameof(CreateWaitableTimerA));

			LARGE_INTEGER liDueTime;
			liDueTime.QuadPart = std::chrono::duration_cast<std::chrono::microseconds>(delay).count() * -10LL;

			const bool success = SetWaitableTimer(this->_handle, &liDueTime, 0, NULL, NULL, 0);

			if (!success)
				throw Windows::Win32Exception(GetLastError(), nameof(SetWaitableTimer));
		}

		TimedResetEvent::TimedResetEvent(const std::chrono::milliseconds delay, std::wstring_view name)
		{
			if (MAX_PATH < name.length())
				throw ArgumentException("Wait handle name too long");

			this->_handle = CreateWaitableTimerW(nullptr, true, name.data());

			if (this->_handle == NULL)
				throw Windows::Win32Exception(GetLastError(), nameof(CreateWaitableTimerW));

			LARGE_INTEGER liDueTime;
			liDueTime.QuadPart = delay.count() * 1000LL;

			const bool success = SetWaitableTimer(this->_handle, &liDueTime, 0, NULL, NULL, 0);

			if (!success)
				throw Windows::Win32Exception(GetLastError(), nameof(SetWaitableTimer));
		}

		TimedResetEvent::TimedResetEvent(TimedResetEvent&& other) noexcept
		{
			this->_handle = other._handle;
			other._handle = INVALID_HANDLE_VALUE;
		}

		TimedResetEvent::~TimedResetEvent() noexcept
		{
			if (this->_handle != INVALID_HANDLE_VALUE)
				CloseHandle(this->_handle);
		}

		uint16_t TimedResetEvent::count_handles() const
		{
			return 1;
		}

		uint16_t TimedResetEvent::copy_handles(HandleArray& dest, const uint16_t index) const
		{
			dest[index] = this->_handle;

			return 1;
		}
	}
}
