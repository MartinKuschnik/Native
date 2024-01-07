#include "InvalidOperationException.h"

#include "Util.h"

namespace Native
{
	InvalidOperationException::InvalidOperationException(std::string&& message) noexcept
		: Exception(nameof(InvalidOperationException), std::move(message))
	{
	}
}