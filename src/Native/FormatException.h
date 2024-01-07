#pragma once

#include "Exception.h"

namespace Native
{
	class FormatException : public Exception
	{
	public:
		FormatException(std::string&& message) noexcept;
	};
}