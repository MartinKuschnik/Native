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
			Handle(Handle&& other) noexcept;

			virtual ~Handle() noexcept;

			bool is_valid() const noexcept;

			Handle duplicate() const;

			operator const HANDLE() const noexcept;

			Handle& operator=(HANDLE&& handle) noexcept;

			Handle& operator=(Handle&& other) noexcept;
		};
	}
}

