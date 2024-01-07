#include "Exception.h"

#include <sstream>

#if _HAS_CXX23
#include <stacktrace>
#endif

namespace Native
{
	Exception::Exception(std::string&& exception_name, std::string&& message) noexcept
		: _message(std::move(message)),
		_exceptionName(std::move(exception_name)),
		_what(BuildWhat(_exceptionName, _message))
	{
	}

	const char* Exception::what() const throw()
	{
		return this->_what.c_str();
	}

	std::string Exception::BuildWhat(const std::string_view exception_name, const std::string_view message)
	{
		std::ostringstream os;
		
		os << exception_name << ": " << message;

#if _HAS_CXX23
		for (const std::stacktrace_entry& stack_entry : std::stacktrace::current(3)) // skip child exception ctor Exception ctor and BuildWhat method
		{
			os << std::endl << "   at " << stack_entry.description();

			if (!stack_entry.source_file().empty())
				os << " in " << stack_entry.source_file() << ":line " << stack_entry.source_line();
		}
#endif

		return os.str();
	}
}
