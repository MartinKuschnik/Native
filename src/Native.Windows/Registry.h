#pragma once

#include <string>

#include "RegistryKey.h"

namespace Native
{
	namespace Windows
	{
		class Registry
		{
		public:
			Registry() = delete; // make class static

			static RegistryKey OpenKey(const HKEY hKey, const std::wstring_view subKey);
		};
	}
}