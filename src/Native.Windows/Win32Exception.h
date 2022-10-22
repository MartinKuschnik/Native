#pragma once

#include <exception>
#include <string>

#include "Util.h"

namespace Native
{
	namespace Windows
	{
		class Win32Exception : public std::exception
		{
		private:
			const unsigned long m_ErrorCode;
			const std::string m_Message;

			static std::string FormatErrorMessage(const unsigned long errorCode, const std::string& methodName);
		public:
			explicit Win32Exception(const unsigned long errorCode, const char* methodName);

			const char* what() const throw () override;
		};
	}
}

