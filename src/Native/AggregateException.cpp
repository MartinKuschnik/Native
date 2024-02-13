#include "AggregateException.h"

#include <sstream>

#include "Util.h"

namespace Native
{
	AggregateException::AggregateException(const std::span<const std::exception_ptr> inner_exceptions) noexcept
		: Exception(nameof(AggregateException), FormatMessage(inner_exceptions)),
		_what(AggregateException::BuildWhat(Exception::what(), inner_exceptions))
	{
	}

	std::string AggregateException::FormatMessage(const std::span<const std::exception_ptr> inner_exceptions)
	{
		std::ostringstream os;

		os <<  "One or more errors occurred.";

		if (inner_exceptions.size() > 0)
		{
			for (const std::exception_ptr& ex_ptr : inner_exceptions)
			{
				try
				{
					std::rethrow_exception(ex_ptr);
				}
				catch (const Native::Exception& ex)
				{
					os << " (" << ex.message() << ")";
				}
				catch (const std::exception& ex)
				{
					os << " (" << ex.what() << ")";
				}
				catch (...)
				{
					os << " (Unknown Error)";
				}
			}
		}

		return os.str();
	}

	std::string AggregateException::BuildWhat(const std::string this_what, const std::span<const std::exception_ptr> inner_exceptions)
	{
		std::ostringstream os;

		os << this_what;

		if (inner_exceptions.size() > 0)
		{
			for (const std::exception_ptr& ex_ptr : inner_exceptions)
			{
				try
				{
					std::rethrow_exception(ex_ptr);
				}
				catch (const std::exception& ex)
				{
					os << "\r\n ---> " << ex.what();
				}
				catch (...)
				{
					os << "\r\n ---> Unknown Error";
				}
			}
		}

		return os.str();
	}
}