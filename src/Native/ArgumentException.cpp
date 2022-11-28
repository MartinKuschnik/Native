#include "ArgumentException.h"

namespace Native
{
	ArgumentException::ArgumentException(const std::string&& message) noexcept
		: _message(message)
	{

	}

	const char* ArgumentException::what() const throw()
	{
		return _message.c_str();
	}
}