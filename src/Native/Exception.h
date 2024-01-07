#pragma once

#include <exception>
#include <string>

namespace Native
{
	class Exception : public std::exception
	{
	public:
		Exception(std::string&& exception_name, std::string&& message) noexcept;

		const char* what() const throw() override;

	protected:
		const std::string _message;


	private:
		const std::string _exceptionName;
		const std::string _what;

		static std::string BuildWhat(const std::string_view exception_name, const std::string_view message);
	};
}
