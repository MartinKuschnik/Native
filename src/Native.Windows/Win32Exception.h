#pragma once

#include <Exception.h>

#include "Util.h"

namespace Native
{
	namespace Windows
	{
		class Win32Exception : public Exception
		{
		public:

			const unsigned long ErrorCode;

			explicit Win32Exception(const unsigned long errorCode, const char* methodName);

		private:

			static std::string FormatErrorMessage(const unsigned long errorCode, const std::string& methodName);
		};
	}
}

