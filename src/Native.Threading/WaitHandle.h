#pragma once

#include <array>
#include <chrono>
#include <optional>
#include <span>

#include <Windows.h>

namespace Native
{
	namespace Threading
	{
		class CancellationToken; // use forward deflation because the WaitHandle includes this header

		class WaitHandle
		{
		public:

			using HandleArray = std::array<HANDLE, MAXIMUM_WAIT_OBJECTS>;

			virtual bool wait_one() const;
			virtual bool wait_one(const std::chrono::milliseconds timeout) const;
			virtual bool wait_one(const CancellationToken& cancellationToken) const;

			static bool WaitAll(const std::span<const std::shared_ptr<WaitHandle>> handles);
			static bool WaitAll(const std::span<const std::shared_ptr<WaitHandle>> handles, const CancellationToken& cancellation_token);

			static uint8_t WaitAny(const std::span<const std::shared_ptr<WaitHandle>> handles);
			static std::optional<uint8_t> WaitAny(const std::span<const std::shared_ptr<WaitHandle>> handles, const CancellationToken& cancellation_token);

		protected:

			virtual uint16_t count_handles() const = 0;
			virtual uint16_t copy_handles(HandleArray& dest, const uint16_t index) const = 0;

			static uint16_t CountHandles(const WaitHandle& handle);
			static uint16_t CountHandles(const std::shared_ptr<const WaitHandle>& handle);

			static uint16_t CopyHandles(const WaitHandle& handle, HandleArray& dest, const uint16_t index);
			static uint16_t CopyHandles(const std::shared_ptr<const WaitHandle>& handle, HandleArray& dest, const uint16_t index);

		private:
			static bool Wait(const HANDLE handle, const std::chrono::milliseconds timeout);
			static std::optional<uint8_t> WaitAny(const HandleArray& handles, const uint16_t handleCount, const std::chrono::milliseconds timeout);
			static bool WaitAll(const HandleArray& handles, const uint16_t handleCount, const std::chrono::milliseconds timeout);
		};
	}
}

