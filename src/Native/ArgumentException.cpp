#include "ArgumentException.h"

#include "Util.h"

namespace Native
{
	ArgumentException::ArgumentException(std::string&& message) noexcept
		: Exception(nameof(ArgumentException), std::move(message))
	{
	}
}