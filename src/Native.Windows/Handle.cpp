#include "Handle.h"

#include "Util.h"
#include "Win32Exception.h"

namespace Native
{
	namespace Windows
	{
		Handle::Handle(const HANDLE handle) noexcept
			:_handle(handle)
		{
		}

		Handle::Handle() noexcept
			: _handle(INVALID_HANDLE_VALUE)
		{
		}

		Handle::Handle(Handle&& other) noexcept
			: _handle(other._handle)
		{
			other._handle = INVALID_HANDLE_VALUE;
		}

		Handle::~Handle()
		{
			if (this->_handle != INVALID_HANDLE_VALUE)
				CloseHandle(this->_handle);
		}

		bool Handle::is_valid() const noexcept
		{
			return this->_handle != INVALID_HANDLE_VALUE;
		}


		Handle Handle::duplicate() const
		{
			const HANDLE curr_proc_handle = GetCurrentProcess();

			HANDLE new_handle;

			bool success = ::DuplicateHandle(curr_proc_handle, this->_handle, curr_proc_handle, &new_handle, false, false, DUPLICATE_SAME_ACCESS);

			if (!success)
				throw Win32Exception(GetLastError(), nameof(DuplicateHandle));

			return new_handle;
		}

		Handle::operator const HANDLE() const noexcept
		{
			return this->_handle;
		}

		Handle& Handle::operator=(HANDLE&& handle) noexcept
		{
			if (this->_handle != INVALID_HANDLE_VALUE)
				CloseHandle(this->_handle);

			this->_handle = handle;

			return *this;
		}

		Handle& Handle::operator=(Handle&& other) noexcept
		{
			if (this->_handle != INVALID_HANDLE_VALUE)
				CloseHandle(this->_handle);

			this->_handle = other._handle;
			other._handle = INVALID_HANDLE_VALUE;

			return *this;
		}
	}
}