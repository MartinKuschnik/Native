#pragma once

#include <stdint.h>
#include <chrono>

#include "SeekOrigin.h"
#include "CancellationToken.h"

namespace Native
{
	namespace IO
	{
		struct Stream
		{
			using Timeout = std::chrono::duration<std::chrono::milliseconds>;

			
			// We pick a value that is the largest multiple of 4096 that is still smaller than the large object heap threshold (85K).
			// The CopyTo/CopyToAsync buffer is short-lived and is likely to be collected at Gen0, and it offers a significant
			// improvement in Copy performance.
			static constexpr inline size_t DefaultCopyBufferSize = 81920;

			virtual ~Stream() = default;

			/// <summary>
			/// Gets a value indicating whether the current stream supports reading.
			/// </summary>
			/// <returns>true if the stream supports reading; otherwise, false.</returns>
			virtual bool can_read() const = 0;

			/// <summary>
			/// Gets a value indicating whether the current stream supports writing.
			/// </summary>
			/// <returns>true if the stream supports writing; otherwise, false.</returns>
			virtual bool can_write() const = 0;

			/// <summary>
			/// Gets a value indicating whether the current stream supports seeking.
			/// </summary>
			/// <returns>true if the stream supports seeking; otherwise, false.</returns>
			virtual bool can_seek() const = 0;

			/// <summary>
			/// Gets the length in bytes of the stream.
			/// </summary>
			/// <returns>The length of the stream in bytes.</returns>
			virtual uint64_t length() const = 0;

			/// <summary>
			/// Gets the position within the current stream.
			/// </summary>
			/// <returns>The current position within the stream.</returns>
			virtual uint64_t position() const = 0;

			/// <summary>
			/// Clears all buffers for this stream and causes any buffered data to be written to the underlying device.
			/// </summary>
			virtual void flush() = 0;

			/// <summary>
			/// Closes the current stream and releases any resources (such as sockets and file handles) associated with the current stream.
			/// </summary>
			virtual void close() = 0;

			/// <summary>
			/// Sets the position within the current stream.
			/// </summary>
			/// <param name="offset">A byte offset relative to the origin parameter.</param>
			/// <param name="origin">A value of type SeekOrigin indicating the reference point used to obtain the new position.</param>
			/// <returns>The new position within the current stream.</returns>
			virtual uint64_t seek(const uint64_t offset, const SeekOrigin origin) = 0;

			/// <summary>
			/// Sets the length of the current stream.
			/// </summary>
			virtual void set_length(const uint64_t length) = 0 ;

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
			virtual size_t read(void* buffer, const size_t buffer_size) = 0;

			/// <summary>
			/// Writes a sequence of bytes to the current stream and advances the current position within this stream by the number of bytes written.
			/// </summary>
			/// <param name="buffer">A region of memory. This method copies the contents of this region to the current stream.</param>
			/// <param name="buffer_size">The size of th buffer.</param>
			virtual void write(const void* buffer, const size_t buffer_size) = 0;

			/// <summary>
			/// Reads the bytes from the current stream and writes them to another stream. Both streams positions are advanced by the number of bytes copied.
			/// </summary>
			/// <param name="destination">The stream to which the contents of the current stream will be copied.</param>
			/// <returns>The number of bytes copied.</returns>
			virtual size_t copy_to(Stream* const destination, const size_t buffer_size = DefaultCopyBufferSize, const Threading::CancellationToken cancellation_token = Threading::CancellationToken::None);
		};
	}
}