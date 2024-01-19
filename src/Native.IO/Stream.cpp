#include "Stream.h"

namespace Native
{
	namespace IO
	{
		size_t Stream::copy_to(Stream* const destination, const size_t buffer_size, const Threading::CancellationToken cancellation_token)
		{
			const auto buffer = std::make_unique_for_overwrite<std::byte[]>(buffer_size);

			size_t read;
			size_t total_copied = 0;

			while (!cancellation_token.is_cancellation_requested() && (read = this->read(buffer.get(), buffer_size)) != 0)
			{
				destination->write(buffer.get(), read);
				total_copied += read;
			}

			return total_copied;
		}
	}
}
