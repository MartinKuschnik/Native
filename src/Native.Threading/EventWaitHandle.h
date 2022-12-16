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

		protected:

			virtual uint16_t count_handles() const override;
			virtual uint16_t copy_handles(HandleArray& dest, const uint16_t index) const override;

		private:
			HANDLE _handle;
		};
	}
}

