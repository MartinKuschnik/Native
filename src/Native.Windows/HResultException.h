#pragma once

#include <Windows.h>

#include <system_error>

#include "Util.h"

namespace Native
{
	namespace Windows
	{
		class HResultException : public std::exception
		{
		private:
			const HRESULT m_HResult;
			const std::string m_Message;

			static std::string FormatErrorMessage(const HRESULT hresult, const std::string& methodName);
		public:
			explicit HResultException(const HRESULT hresult, const std::string& methodName);

			const char* what() const throw () override;
		};
	}

}