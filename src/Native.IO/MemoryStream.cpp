#include "MemoryStream.h"

#include "InvalidOperationException.h"
#include "NotSupportedException.h"

namespace Native
{
	namespace IO
	{
		MemoryStream::MemoryStream() noexcept
			: _data(nullptr),
			_size(0),
			_writable(false),
			_position(0)
		{
		}

		MemoryStream::MemoryStream(const uint64_t size, const bool writable)
			: _data(new std::byte[size]),
			_size(size),
			_writable(writable),
			_position(0)
		{
		}

		MemoryStream::~MemoryStream() noexcept
		{
			if (_data != nullptr)
				delete[] _data;
		}

		uint64_t MemoryStream::seek(const uint64_t offset, const SeekOrigin origin)
		{
			uint64_t new_pos;

			switch (origin)
			{
			case SeekOrigin::Begin:

				new_pos = offset;
				break;

			case SeekOrigin::End:

				new_pos = this->_size - offset;
				break;

			default:

				new_pos = this->_position + offset;
				break;
			}

			if (new_pos < 0 || new_pos > this->_size)
				throw Native::InvalidOperationException("Invalid offset.");

			return this->_position = new_pos;
		}

		uint64_t MemoryStream::read(void* buffer, const uint64_t buffer_size)
		{
			const uint64_t bytes_to_read = std::min(buffer_size, this->_size - this->_position);

			std::memcpy(buffer, this->_data + this->_position, bytes_to_read);

			this->_position += bytes_to_read;

			return bytes_to_read;
		}

		void MemoryStream::write(const void* buffer, const uint64_t buffer_size)
		{
			if (this->_size - this->_position < buffer_size)
				throw Native::InvalidOperationException("To much data.");

			std::memcpy(this->_data + this->_position, buffer, buffer_size);
		}
	}
}