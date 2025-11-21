#include "NamedPipeClientStream.h"

#include <format>

#include "NotSupportedException.h"
#include "Win32Exception.h"

using namespace std::chrono_literals;

namespace Native
{
	using namespace Diagnostics;
	using namespace Windows;

	namespace IO
	{
		namespace Pipes
		{
			NamedPipeClientStream::NamedPipeClientStream(const std::wstring_view pipe_name) noexcept
				: NamedPipeClientStream(L".", pipe_name)
			{
			}

			NamedPipeClientStream::NamedPipeClientStream(const std::wstring_view server_name, const std::wstring_view pipe_name) noexcept
				: NamedPipeClientStream(server_name, pipe_name, PipeDirection::InOut)
			{
			}

			NamedPipeClientStream::NamedPipeClientStream(const std::wstring_view server_name, const std::wstring_view pipe_name, const PipeAccessRights desired_access_rights) noexcept
				: _fullPipeName(std::format(L"\\\\{0}\\pipe\\{1}", server_name, pipe_name)),
				_accessRights(static_cast<DWORD>(desired_access_rights))
			{
			}

			NamedPipeClientStream::NamedPipeClientStream(const std::wstring_view server_name, const std::wstring_view pipe_name, const PipeDirection pipe_direction) noexcept
				: _fullPipeName(std::format(L"\\\\{0}\\pipe\\{1}", server_name, pipe_name)),
				_accessRights(AccessRightsFromDirection(pipe_direction))
			{
			}

			bool NamedPipeClientStream::Connect(std::chrono::milliseconds timeout)
			{
				this->_pipeHandle = CreateFileW(
					this->_fullPipeName.c_str(),
					this->_accessRights,
					NULL,           // no sharing 
					nullptr,        // default security attributes
					OPEN_EXISTING,  // opens existing pipe 
					NULL,           // default attributes 
					NULL);          // no template file 

				if (this->_pipeHandle.is_valid())
					return true;

				DWORD last_error = GetLastError();

				switch (last_error)
				{
				case ERROR_FILE_NOT_FOUND:
					return false;
				case ERROR_PIPE_BUSY:
				{
					if (timeout == 0ms)
						return false;

					const bool pipe_ready = WaitNamedPipeW(this->_fullPipeName.c_str(), static_cast<DWORD>(timeout.count()));

					if (pipe_ready)
						return this->Connect(0ms);

					last_error = GetLastError();

					switch (last_error)
					{
					case ERROR_SEM_TIMEOUT:
						return false;
					case ERROR_FILE_NOT_FOUND:
						return false;
					case ERROR_PIPE_BUSY:
						return false;
					default:
						throw Win32Exception(last_error, nameof(WaitNamedPipeW));
					}
				}
				default:
					throw Win32Exception(last_error, nameof(CreateFileW));
				}
			}

			bool NamedPipeClientStream::can_read() const
			{
				return (this->_accessRights & GENERIC_READ) == GENERIC_READ
					|| (this->_accessRights & static_cast<DWORD>(PipeAccessRights::Read)) == static_cast<DWORD>(PipeAccessRights::Read);
			}

			bool NamedPipeClientStream::can_write() const
			{
				return (this->_accessRights & GENERIC_WRITE) == GENERIC_WRITE 
					|| (this->_accessRights & static_cast<DWORD>(PipeAccessRights::Write)) == static_cast<DWORD>(PipeAccessRights::Write);
			}

			bool NamedPipeClientStream::can_seek() const
			{
				return false;
			}

			uint64_t NamedPipeClientStream::length() const
			{
				throw NotSupportedException("NamedPipeClientStream does not support querying the length.");
			}

			uint64_t NamedPipeClientStream::position() const
			{
				throw NotSupportedException("NamedPipeClientStream does not support querying the position.");
			}

			void NamedPipeClientStream::flush()
			{
				const bool success = FlushFileBuffers(this->_pipeHandle);

				if (!success)
					throw Native::Windows::Win32Exception(GetLastError(), nameof(FlushFileBuffers));
			}

			void NamedPipeClientStream::close()
			{
				this->_pipeHandle = INVALID_HANDLE_VALUE;
			}

			uint64_t NamedPipeClientStream::seek(const uint64_t offset, const SeekOrigin origin)
			{
				throw NotSupportedException("NamedPipeClientStream does not support seeking.");
			}

			void NamedPipeClientStream::set_length(const uint64_t length)
			{
				throw NotSupportedException("NamedPipeClientStream does not support setting the length.");
			}

			size_t NamedPipeClientStream::read(void* buffer, const size_t buffer_size)
			{
				DWORD read;

				const bool success = ReadFile(this->_pipeHandle, buffer, static_cast<DWORD>(buffer_size), &read, nullptr);

				if (!success)
					throw Native::Windows::Win32Exception(GetLastError(), nameof(ReadFile));

				return read;
			}

			void NamedPipeClientStream::write(const void* buffer, const size_t buffer_size)
			{
				DWORD total_written = 0;

				do
				{
					DWORD written;
					const bool success = WriteFile(this->_pipeHandle, static_cast<const std::byte*>(buffer) + total_written, static_cast<DWORD>(buffer_size - total_written), &written, nullptr);

					if (!success)
						throw Native::Windows::Win32Exception(GetLastError(), nameof(WriteFile));

					total_written += written;

				} while (total_written < buffer_size);
			}

			constexpr DWORD NamedPipeClientStream::AccessRightsFromDirection(PipeDirection direction) noexcept
			{
				switch (direction)
				{
				case Native::IO::Pipes::PipeDirection::In:
					return GENERIC_READ;
				case Native::IO::Pipes::PipeDirection::Out:
					return GENERIC_WRITE;
				case Native::IO::Pipes::PipeDirection::InOut:
					return GENERIC_READ | GENERIC_WRITE;
				default:
					return NULL;
				}
			}
		}
	}
}
