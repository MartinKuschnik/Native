#include "Win32Exception.h"

#include <iomanip>
#include <sstream>

#include "Win32.h"

namespace Native
{
	namespace Windows
	{
		Win32Exception::Win32Exception(const unsigned long errorCode, const char* methodName)
			: Native::Exception(nameof(Win32Exception), Win32Exception::FormatErrorMessage(errorCode, methodName)),
			  m_ErrorCode(errorCode)
		{
		}

		std::string Win32Exception::FormatErrorMessage(const unsigned long errorCode, const std::string& methodName)
		{
			std::ostringstream os;
			os << std::hex << std::setfill('0');

			std::string errorMessage = Win32::GetWin32ErrorMessage(errorCode);

			os << methodName << "(...) failed! (Win32ErrorCode=0x" << std::uppercase << std::hex << std::setw(8) << errorCode << std::nouppercase;

			if (errorMessage.size() > 0)
				os << ": " << errorMessage;

			os << ")";

			return os.str();
		}
	}
}