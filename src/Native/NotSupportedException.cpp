#include "NotSupportedException.h"

#include "Util.h"

namespace Native
{
	NotSupportedException::NotSupportedException(std::string&& message) noexcept
		: Exception(nameof(NotSupportedException), std::move(message))
	{
	}
}