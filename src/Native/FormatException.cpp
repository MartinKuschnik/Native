#include "FormatException.h"

namespace Native
{
	FormatException::FormatException(const std::string&& message) noexcept
		: _message(message)
	{

	}

	const char* FormatException::what() const throw()
	{
		return _message.c_str();
	}
}