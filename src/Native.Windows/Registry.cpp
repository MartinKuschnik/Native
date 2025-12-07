#include "Registry.h"

#include "Win32Exception.h"

namespace Native
{
	namespace Windows
	{
		RegistryKey Registry::OpenKey(const HKEY hKey, const std::wstring_view subKey)
		{
			HKEY hSubKey = nullptr;

			const LONG error = RegOpenKeyExW(hKey, subKey.data(), 0, KEY_READ, &hSubKey);

			switch (error)
			{
			case ERROR_SUCCESS:
				return RegistryKey(hSubKey);
			case ERROR_FILE_NOT_FOUND:
				return RegistryKey(nullptr);
			default:
				throw Win32Exception(error, nameof(RegOpenKeyExW));
			}
		}
	}
}