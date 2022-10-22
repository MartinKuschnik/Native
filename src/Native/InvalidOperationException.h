#pragma once

#pragma once

#include <exception>
#include <string>

namespace Native
{
	class InvalidOperationException : public std::exception
	{

	public:
		InvalidOperationException(const std::string&& message) noexcept;

		const char* what() const throw() override;

	private:
		const std::string _message;
	};
}