#include "FileStream.h"

#include <Windows.h>

#include "NotSupportedException.h"
#include "Win32Exception.h"

namespace Native
{
	namespace IO
	{
		FileStream::FileStream(const std::filesystem::path path, const FileMode mode, const FileAccess access, const FileShare share)
			: _handle(OpenFile(path, mode, access, share)),
			_access(access)
		{
		}

		uint64_t FileStream::length() const
		{
			LARGE_INTEGER value;

			const bool success = GetFileSizeEx(this->_handle, &value);

			if (!success)
				throw Native::Windows::Win32Exception(GetLastError(), nameof(GetFileSizeEx));

			return value.QuadPart;
		}

		uint64_t FileStream::position() const
		{
			LARGE_INTEGER value;

			const bool success = SetFilePointerEx(this->_handle, LARGE_INTEGER{0}, & value, FILE_CURRENT);

			if (!success)
				throw Native::Windows::Win32Exception(GetLastError(), nameof(SetFilePointerEx));

			return value.QuadPart;
		}


		void FileStream::flush()
		{
			const bool success = FlushFileBuffers(this->_handle);

			if (!success)
				throw Native::Windows::Win32Exception(GetLastError(), nameof(FlushFileBuffers));
		}


		void FileStream::close()
		{
			this->_handle = INVALID_HANDLE_VALUE;
		}

		uint64_t FileStream::seek(const uint64_t offset, const SeekOrigin origin)
		{
			LARGE_INTEGER new_value;

			const bool success = SetFilePointerEx(this->_handle, LARGE_INTEGER { .QuadPart = static_cast<const LONGLONG>(offset)}, & new_value, static_cast<DWORD>(origin));

			if (!success)
				throw Native::Windows::Win32Exception(GetLastError(), nameof(SetFilePointerEx));

			return new_value.QuadPart;
		}


		void FileStream::set_length(const uint64_t length)
		{
			const uint64_t current_pos = this->position();

			this->seek(length, SeekOrigin::Begin);			

			const bool success = SetEndOfFile(this->_handle);

			if (current_pos < length)
				this->seek(current_pos, SeekOrigin::Begin);

			if (!success)
				throw Native::Windows::Win32Exception(GetLastError(), nameof(SetEndOfFile));
		}


		uint64_t FileStream::read(void* buffer, const uint64_t buffer_size)
		{
			DWORD read;

			if (buffer_size > 0xffffffffUL)
				throw Native::NotSupportedException("Buffer to big.");

			const bool success = ReadFile(this->_handle, buffer, static_cast<DWORD>(buffer_size), &read, nullptr);

			if (!success)
				throw Native::Windows::Win32Exception(GetLastError(), nameof(ReadFile));

			return read;
		}

		void FileStream::write(const void* buffer, const uint64_t buffer_size)
		{
			DWORD total_written = 0;

			if (buffer_size > 0xffffffffUL)
				throw Native::NotSupportedException("Buffer to big.");

			do
			{
				DWORD written;
				const bool success = WriteFile(this->_handle, static_cast<const std::byte*>(buffer) + total_written, static_cast<DWORD>(buffer_size - total_written), &written, nullptr);

				if (!success)
					throw Native::Windows::Win32Exception(GetLastError(), nameof(WriteFile));

				total_written += written;

			} while (total_written < buffer_size);
		}

		Native::Windows::Handle FileStream::OpenFile(const std::filesystem::path path, const FileMode mode, const FileAccess access, const FileShare share)
		{
			const std::wstring path_as_wstring = path.wstring();

			HANDLE handle = CreateFileW(path_as_wstring.c_str(), static_cast<DWORD>(access), static_cast<DWORD>(share), nullptr, static_cast<DWORD>(mode), FILE_ATTRIBUTE_NORMAL, nullptr);

			if (handle == INVALID_HANDLE_VALUE)
				throw Native::Windows::Win32Exception(GetLastError(), nameof(CreateFileW));

			return Native::Windows::Handle(handle);
		}
	}
}