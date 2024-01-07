#pragma once

#include <Windows.h>
#include <cfgmgr32.h>

#include "Exception.h"

#include "Util.h"

namespace Native
{
	namespace Windows
	{
		class ConfigurationManagerReturnCodeException : public Exception
		{
		private:
			const CONFIGRET _returnCode;

			static std::string FormatErrorMessage(const CONFIGRET returnCode, const std::string& methodName);

		public:
			explicit ConfigurationManagerReturnCodeException(const CONFIGRET returnCode, const char* methodName);
		};
	}
}



