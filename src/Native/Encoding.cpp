#include "Encoding.h"

#include <Windows.h>

#include "Win32Exception.h"

namespace Native
{
	using namespace Windows;

	std::string Encoding::UTF8::Encode(const std::wstring_view wstr)
	{
		if (wstr.empty())
			return std::string();

		int newLength = WideCharToMultiByte(CP_UTF8, 0, wstr.data(), static_cast<DWORD>(wstr.length()), nullptr, 0, nullptr, nullptr);

		if (newLength == 0)
			throw Win32Exception(GetLastError(), nameof(WideCharToMultiByte));

		std::string val(newLength, 0);

		newLength = WideCharToMultiByte(CP_UTF8, 0, wstr.data(), static_cast<DWORD>(wstr.length()), val.data(), newLength, nullptr, nullptr);

		if (newLength == 0)
			throw Win32Exception(GetLastError(), nameof(WideCharToMultiByte));

		return val;
	}

	std::wstring Encoding::UTF8::Decode(const std::string_view str)
	{
		if (str.empty())
			return std::wstring();

		int newLength = MultiByteToWideChar(CP_UTF8, 0, str.data(), static_cast<DWORD>(str.length()), nullptr, 0);

		if (newLength == 0)
			throw Win32Exception(GetLastError(), nameof(MultiByteToWideChar));

		std::wstring val(newLength, 0);

		newLength = MultiByteToWideChar(CP_UTF8, 0, str.data(), static_cast<DWORD>(str.length()), val.data(), newLength);

		if (newLength == 0)
			throw Win32Exception(GetLastError(), nameof(MultiByteToWideChar));

		return val;
	}
}