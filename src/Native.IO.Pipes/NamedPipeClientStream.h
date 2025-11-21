#pragma once

#include <chrono>
#include <string>

#include <windows.h>

#include "Handle.h"
#include "PipeDirection.h"
#include "PipeAccessRights.h"
#include "Stream.h"

namespace Native
{
	namespace IO
	{
		namespace Pipes
		{
			class NamedPipeClientStream : public Native::IO::Stream
			{
			public:
				NamedPipeClientStream(const std::wstring_view pipe_name) noexcept;

				NamedPipeClientStream(const std::wstring_view server_name, const std::wstring_view pipe_name) noexcept;

				NamedPipeClientStream(const std::wstring_view server_name, const std::wstring_view pipe_name, const PipeAccessRights desired_access_rights) noexcept;

				NamedPipeClientStream(const std::wstring_view server_name, const std::wstring_view pipe_name, const PipeDirection pipe_direction) noexcept;

				virtual ~NamedPipeClientStream() noexcept = default;

				[[nodiscard]]
				bool Connect(std::chrono::milliseconds timeout);

				/// <summary>
				/// Gets a value indicating whether the current stream supports reading.
				/// </summary>
				/// <returns>true if the stream supports reading; otherwise, false.</returns>
				bool can_read() const override;

				/// <summary>
				/// Gets a value indicating whether the current stream supports writing.
				/// </summary>
				/// <returns>true if the stream supports writing; otherwise, false.</returns>
				bool can_write() const override;

				/// <summary>
				/// Gets a value indicating whether the current stream supports seeking.
				/// </summary>
				/// <returns>true if the stream supports seeking; otherwise, false.</returns>
				bool can_seek() const override;

				/// <summary>
				/// Gets the length in bytes of the stream.
				/// </summary>
				/// <returns>The length of the stream in bytes.</returns>
				uint64_t length() const override;

				/// <summary>
				/// Gets the position within the current stream.
				/// </summary>
				/// <returns>The current position within the stream.</returns>
				uint64_t position() const override;

				/// <summary>
				/// Clears all buffers for this stream and causes any buffered data to be written to the underlying device.
				/// </summary>
				void flush() override;

				/// <summary>
				/// Closes the current stream and releases any resources (such as sockets and file handles) associated with the current stream.
				/// </summary>
				void close() override;

				/// <summary>
				/// Sets the position within the current stream.
				/// </summary>
				/// <param name="offset">A byte offset relative to the origin parameter.</param>
				/// <param name="origin">A value of type SeekOrigin indicating the reference point used to obtain the new position.</param>
				/// <returns>The new position within the current stream.</returns>
				uint64_t seek(const uint64_t offset, const SeekOrigin origin) override;

				/// <summary>
				/// Sets the length of the current stream.
				/// </summary>
				void set_length(const uint64_t length) override;

				/// <summary>
				/// Reads a sequence of bytes from the current stream and advances the position within the stream by the number of bytes read.
				/// </summary>
				/// <param name="buffer">A region of memory. When this method returns, the contents of this region are replaced by the bytes read from the current source.</param>
				/// <param name="buffer_size">The size of th buffer.</param>
				/// <returns>
				/// The total number of bytes read into the buffer. 
				/// This can be less than the size of the buffer if that many bytes are not currently available,
				/// or zero (0) if the buffer's length is zero or the end of the stream has been reached.
				/// </returns>
				size_t read(void* buffer, const size_t buffer_size) override;

				/// <summary>
				/// Writes a sequence of bytes to the current stream and advances the current position within this stream by the number of bytes written.
				/// </summary>
				/// <param name="buffer">A region of memory. This method copies the contents of this region to the current stream.</param>
				/// <param name="buffer_size">The size of th buffer.</param>
				void write(const void* buffer, const size_t buffer_size) override;

			private:

				const std::wstring _fullPipeName;
				const DWORD _accessRights;

				Windows::Handle _pipeHandle;

				static constexpr DWORD AccessRightsFromDirection(PipeDirection direction) noexcept;
			};
		}
	}

}