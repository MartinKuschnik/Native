#include "Handle.h"

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

		Handle::operator const HANDLE() const noexcept
		{
			return this->_handle;
		}

		Handle& Handle::operator=(const HANDLE& handle) noexcept
		{
			if (this->_handle != INVALID_HANDLE_VALUE)
				CloseHandle(this->_handle);

			this->_handle = handle;

			return *this;
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