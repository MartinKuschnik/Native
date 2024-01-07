#pragma once

#include <Windows.h>

#include "Exception.h"

#include "Util.h"

namespace Native
{
	namespace Windows
	{
		class HResultException : public Exception
		{
		private:
			const HRESULT m_HResult;

			static std::string FormatErrorMessage(const HRESULT hresult, const std::string& methodName);
		public:
			explicit HResultException(const HRESULT hresult, const std::string& methodName);
		};
	}

}