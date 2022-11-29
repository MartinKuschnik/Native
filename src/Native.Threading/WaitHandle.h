#pragma once

#include <chrono>

#include <Windows.h>

namespace Native
{
	namespace Threading
	{
		class WaitHandle
		{
		public:
			virtual bool wait_one() const = 0;

			virtual bool wait_one(const std::chrono::milliseconds timeout) const = 0;

		protected:

			virtual HANDLE handle() const = 0;
		};
	}
}

