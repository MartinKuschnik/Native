#pragma once

#include "Exception.h"


namespace Native
{
	class NotSupportedException : public Exception
	{

	public:
		NotSupportedException(std::string&& message) noexcept;
	};
}