#include "FormatException.h"

#include "Util.h"

namespace Native
{
	FormatException::FormatException(std::string&& message) noexcept
		: Native::Exception(nameof(FormatException), std::move(message))
	{

	}
}