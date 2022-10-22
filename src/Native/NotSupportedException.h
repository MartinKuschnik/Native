#pragma once

#include <exception>
#include <string>

namespace Native
{
	class NotSupportedException : public std::exception
	{

	public:
		NotSupportedException(const std::string&& message) noexcept;

		const char* what() const throw() override;

	private:
		const std::string _message;
	};
}