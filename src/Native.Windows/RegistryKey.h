#pragma once

#include <string>

#include <Windows.h>

namespace Native
{
	namespace Windows
	{
		class RegistryKey
		{
		private:
			const HKEY m_hKey;

		public:
			RegistryKey() noexcept = delete;
			RegistryKey(const HKEY hKey) noexcept;
			~RegistryKey() noexcept = default;

			const bool exists() const noexcept;

			template<typename T>
			const T GetValue(const std::wstring_view valueName, const T defaultValue) const;

			const std::wstring GetValue(const std::wstring_view valueName, const wchar_t* defaultValue) const;

			const bool HasValue(const std::wstring_view valueName) const;
		};
	}
}