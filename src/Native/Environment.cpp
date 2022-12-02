#include "Environment.h"

#include <Windows.h>

#include <dsgetdc.h>
#include <lm.h>

#include "Win32Exception.h"

namespace Native
{
	using namespace Windows;

	std::string Environment::Username()
	{
		std::string username(UNLEN, '\0');

		DWORD size = static_cast<DWORD>(username.length());

		GetUserNameA(username.data(), &size);

		username.resize(size > 0 ? size - 1 : 0);

		return username;
	}

	std::string Environment::ComputerName(const COMPUTER_NAME_FORMAT format)
	{
		DWORD size = 0;

		GetComputerNameExA(format, nullptr, &size);

		std::string computerName(size, L'\0');

		GetComputerNameExA(format, computerName.data(), &size);

		computerName.resize(size > 0 ? size : 0);

		return computerName;
	}
}
