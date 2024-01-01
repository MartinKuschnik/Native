#pragma once

#include <Windows.h>

namespace Native
{
	namespace Windows
	{
		class Handle
		{

		private:
			HANDLE _handle;

		public:
			Handle(const HANDLE handle) noexcept;

			Handle() noexcept;
			Handle(const Handle&) = delete;
			Handle(Handle&&) noexcept;

			virtual ~Handle() noexcept;

			bool is_valid() const noexcept;

			operator const HANDLE() const noexcept;

			Handle& operator=(const HANDLE& handle) noexcept;
			Handle& operator=(HANDLE&& handle) noexcept;

			Handle& operator=(Handle&& other) noexcept;
		};
	}
}

