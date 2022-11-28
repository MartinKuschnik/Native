#pragma once

#include <exception>
#include <string>

namespace Native
{
	class ArgumentException : public std::exception
	{

	public:
		ArgumentException(const std::string&& message) noexcept;

		const char* what() const throw() override;

	private:
		const std::string _message;
	};
}

