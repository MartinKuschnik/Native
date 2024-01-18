#include "Stream.h"

namespace Native
{
	namespace IO
	{
		void Stream::copy_to(Stream* destination, const size_t buffer_size)
		{
			std::vector<std::byte> buffer(buffer_size);

			size_t read;

			while ((read = this->read(buffer.data(), buffer_size)) != 0)
				destination->write(buffer.data(), buffer_size);
		}
	}
}
