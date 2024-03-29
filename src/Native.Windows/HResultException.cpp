#include "HResultException.h"

#include <iomanip>
#include <sstream>

#include "Win32.h"

namespace Native
{
	namespace Windows
	{
		HResultException::HResultException(const HRESULT hresult, const std::string& methodName)
			: Exception(nameof(HResultException), HResultException::FormatErrorMessage(hresult, methodName)),
			m_HResult(hresult)
		{
		}

		std::string HResultException::FormatErrorMessage(const HRESULT hresult, const std::string& methodName)
		{
			std::ostringstream os;
			os << std::hex << std::setfill('0');

			std::string errorMessage = Win32::GetWin32ErrorMessage(hresult);


			os << methodName << "(...) failed! (HRESULT=0x" << std::uppercase << std::hex << std::setw(8) << hresult << std::nouppercase;

			if (errorMessage.size() > 0)
				os << ": " << errorMessage;

			os << ")";

			return os.str();
		}
	}
}
