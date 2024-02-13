#pragma once

#include "Exception.h"

#include <span>

namespace Native
{
	class AggregateException : public Exception
	{
	public:
		AggregateException(const std::span<const std::exception_ptr> inner_exceptions) noexcept;

		constexpr const char* what() const throw() override
		{
			return this->_what.c_str();
		}

	private:
		const std::string _what;

		static std::string BuildWhat(const std::string this_what, const std::span<const std::exception_ptr> inner_exceptions);

		static std::string FormatMessage(const std::span<const std::exception_ptr> inner_exceptions);
	};
}
