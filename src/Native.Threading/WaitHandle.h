#pragma once

#include <array>
#include <chrono>

#include <Windows.h>

namespace Native
{
	namespace Threading
	{
		class WaitHandle
		{
		public:

			using HandleArray = std::array<HANDLE, MAXIMUM_WAIT_OBJECTS>;

			virtual bool wait_one() const;

			virtual bool wait_one(const std::chrono::milliseconds timeout) const;

		protected:

			virtual uint16_t count_handles() const = 0;
			virtual uint16_t copy_handles(HandleArray& dest, const uint16_t index) const = 0;

			static uint16_t CountHandles(const WaitHandle& handle);
			static uint16_t CountHandles(const std::shared_ptr<WaitHandle>& handle);

			static uint16_t CopyHandles(const WaitHandle& handle, HandleArray& dest, const uint16_t index);
			static uint16_t CopyHandles(const std::shared_ptr<WaitHandle>& handle, HandleArray& dest, const uint16_t index);

		private:
			static bool WaitOne(const HANDLE handle, const std::chrono::milliseconds timeout);
			static bool WaitOne(const HandleArray& handles, const uint16_t handleCount, const std::chrono::milliseconds timeout);
		};
	}
}

