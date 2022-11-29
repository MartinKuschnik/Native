#pragma once

#include <string_view>

#include <Windows.h>

#include "EventResetMode.h"
#include "WaitHandle.h"

namespace Native
{
	namespace Threading
	{
		class EventWaitHandle : public WaitHandle
		{
		public:

			EventWaitHandle(bool initialState, EventResetMode mode) noexcept;
			EventWaitHandle(bool initialState, EventResetMode mode, std::string_view name);
			EventWaitHandle(bool initialState, EventResetMode mode, std::wstring_view name);
		
			EventWaitHandle(const EventWaitHandle&) = delete;
			EventWaitHandle(EventWaitHandle&& other) noexcept;

			~EventWaitHandle() noexcept;

			void reset();
			void set();

			bool wait_one() const override;

			bool wait_one(const std::chrono::milliseconds timeout) const override;

		protected:

		 HANDLE handle() const override;

		private:
			HANDLE _handle;
		};
	}
}

