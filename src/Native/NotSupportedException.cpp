#include "NotSupportedException.h"

namespace Native
{
	NotSupportedException::NotSupportedException(const std::string&& message) noexcept
		: _message(message)
	{

	}

	const char* NotSupportedException::what() const throw()
	{
		return _message.c_str();
	}
}