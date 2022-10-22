#pragma once

#include <exception>
#include <string>

namespace Native
{
	class FormatException : public std::exception
	{

	public:
		FormatException(const std::string&& message) noexcept;

		const char* what() const throw() override;

	private:
		const std::string _message;
	};
}