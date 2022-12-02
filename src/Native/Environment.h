#pragma once

#include <Windows.h>

#include <string>

namespace Native
{
	class Environment
	{
	public:
		Environment() = delete;

		static std::string Username();

		static std::string ComputerName(const COMPUTER_NAME_FORMAT format);
	};
}