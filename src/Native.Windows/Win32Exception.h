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
			explicit Win32Exception(const unsigned long errorCode, const char* methodName);

		private:
			const unsigned long m_ErrorCode;

			static std::string FormatErrorMessage(const unsigned long errorCode, const std::string& methodName);
		};
	}
}

