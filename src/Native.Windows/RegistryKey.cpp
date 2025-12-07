#include "RegistryKey.h"

#include <vector>

#include "Registry.h"
#include "Win32Exception.h"

namespace Native
{
	namespace Windows
	{
		RegistryKey::RegistryKey(const HKEY hKey) noexcept
			: m_hKey(hKey)
		{
		}

		const bool RegistryKey::exists() const noexcept
		{
			return this->m_hKey != nullptr;
		}

		template<>
		const DWORD RegistryKey::GetValue(const std::wstring_view valueName, const DWORD defaultValue) const
		{
			if (!this->exists())
				return defaultValue;

			DWORD dwBufferSize(sizeof(DWORD));
			DWORD result = 0;

			const LONG error = RegQueryValueExW(this->m_hKey, valueName.data(), nullptr, nullptr, reinterpret_cast<LPBYTE>(&result), &dwBufferSize);

			switch (error)
			{
			case ERROR_SUCCESS:
				return result;
			case ERROR_FILE_NOT_FOUND:
				return defaultValue;
			default:
				throw Win32Exception(error, nameof(RegQueryValueExW));
			}
		}


		template<>
		const bool RegistryKey::GetValue(const std::wstring_view valueName, const bool defaultValue) const
		{
			if (!this->exists())
				return defaultValue;

			DWORD dwBufferSize(sizeof(DWORD));
			DWORD result = 0;

			const LONG error = RegQueryValueExW(this->m_hKey, valueName.data(), nullptr, nullptr, reinterpret_cast<LPBYTE>(&result), &dwBufferSize);

			switch (error)
			{
			case ERROR_SUCCESS:
				return result;
			case ERROR_FILE_NOT_FOUND:
				return defaultValue;
			default:
				throw Win32Exception(error, nameof(RegQueryValueExW));
			}
		}

		template<>
		const std::vector<BYTE> RegistryKey::GetValue(const std::wstring_view valueName, const std::vector<BYTE> defaultValue) const
		{
			if (!this->exists())
				return defaultValue;

			DWORD dwBufferSize = 0;

			LONG error = RegQueryValueExW(this->m_hKey, valueName.data(), nullptr, nullptr, nullptr, &dwBufferSize);

			switch (error)
			{
			case ERROR_SUCCESS:
			{
				std::vector<BYTE> result(dwBufferSize);

				error = RegQueryValueExW(this->m_hKey, valueName.data(), nullptr, nullptr, result.data(), &dwBufferSize);

				switch (error)
				{
				case ERROR_SUCCESS:
					return result;
				default:
					throw Win32Exception(error, nameof(RegQueryValueExW));
				}

			}
			case ERROR_FILE_NOT_FOUND:
				return defaultValue;
			default:
				throw Win32Exception(error, nameof(RegQueryValueExW));
			}
		}

		template<>
		const std::wstring RegistryKey::GetValue(const std::wstring_view valueName, const std::wstring defaultValue) const
		{
			return RegistryKey::GetValue(valueName, defaultValue.data());
		}

		const std::wstring RegistryKey::GetValue(const std::wstring_view valueName, const wchar_t* defaultValue) const
		{
			if (!this->exists())
				return std::wstring(defaultValue);

			DWORD dwBufferSize = 0;

			LONG error = RegQueryValueExW(this->m_hKey, valueName.data(), nullptr, nullptr, nullptr, &dwBufferSize);

			switch (error)
			{
			case ERROR_SUCCESS:
			{
				std::wstring result(dwBufferSize / sizeof(wchar_t) - 1, L'\0');

				error = RegQueryValueExW(this->m_hKey, valueName.data(), nullptr, nullptr, reinterpret_cast<LPBYTE>(result.data()), &dwBufferSize);

				switch (error)
				{
				case ERROR_SUCCESS:
					return result;
				default:
					throw Win32Exception(error, nameof(RegQueryValueExW));
				}

			}
			case ERROR_FILE_NOT_FOUND:
				return std::wstring(defaultValue);
			default:
				throw Win32Exception(error, nameof(RegQueryValueExW));
			}
		}

		const bool RegistryKey::HasValue(const std::wstring_view valueName) const
		{
			if (!this->exists())
				return false;

			DWORD dwBufferSize = 0;

			const LONG error = RegQueryValueExW(this->m_hKey, valueName.data(), nullptr, nullptr, nullptr, &dwBufferSize);

			switch (error)
			{
			case ERROR_SUCCESS:
				return true;
			case ERROR_FILE_NOT_FOUND:
				return false;
			default:
				throw Win32Exception(error, nameof(RegQueryValueExW));
			}
		}
	}
}