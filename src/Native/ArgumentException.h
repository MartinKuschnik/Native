#pragma once

#include "Exception.h"

namespace Native
{
	class ArgumentException : public Exception
	{
	public:
		ArgumentException(std::string&& message) noexcept;
	};
}

