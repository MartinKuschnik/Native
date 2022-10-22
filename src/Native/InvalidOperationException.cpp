#include "InvalidOperationException.h"

namespace Native
{
	InvalidOperationException::InvalidOperationException(const std::string&& message) noexcept
		: _message(message)
	{

	}

	const char* InvalidOperationException::what() const throw()
	{
		return _message.c_str();
	}
}