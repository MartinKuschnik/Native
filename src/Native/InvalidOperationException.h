#pragma once

#include "Exception.h"

namespace Native
{
	class InvalidOperationException : public Exception
	{
	public:
		InvalidOperationException(std::string&& message) noexcept;
	};
}