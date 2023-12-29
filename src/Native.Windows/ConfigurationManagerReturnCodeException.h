#pragma once

#include <exception>
#include <string>

#include <Windows.h>
#include <cfgmgr32.h>

#include "Util.h"

namespace Native
{
	namespace Windows
	{
		class ConfigurationManagerReturnCodeException : public std::exception
		{
		private:
			const CONFIGRET _returnCode;
			const std::string _message;

			static std::string FormatErrorMessage(const CONFIGRET returnCode, const std::string& methodName);

		public:
			explicit ConfigurationManagerReturnCodeException(const CONFIGRET returnCode, const char* methodName);

			const char* what() const throw () override;
		};
	}
}



