
#include <chrono>

#include "WaitHandle.h"

#include "EventResetMode.h"

namespace Native
{
	namespace Threading
	{
		class TimedResetEvent : public WaitHandle
		{
		public:

			TimedResetEvent(const std::chrono::milliseconds delay) noexcept;
			TimedResetEvent(const std::chrono::milliseconds delay, std::string_view name);
			TimedResetEvent(const std::chrono::milliseconds delay, std::wstring_view name);

			TimedResetEvent(const std::chrono::system_clock::time_point dueTime) noexcept;
			TimedResetEvent(const std::chrono::system_clock::time_point dueTime, std::string_view name);
			TimedResetEvent(const std::chrono::system_clock::time_point dueTime, std::wstring_view name);
			

			TimedResetEvent(const TimedResetEvent&) = delete;
			TimedResetEvent(TimedResetEvent&& other) noexcept;

			~TimedResetEvent() noexcept;

		protected:

			virtual uint16_t count_handles() const override;
			virtual uint16_t copy_handles(HandleArray& dest, const uint16_t index) const override;

		private:
			HANDLE _handle;
		};
	}
}

