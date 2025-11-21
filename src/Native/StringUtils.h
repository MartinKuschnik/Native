#pragma once

#include <algorithm>
#include <string>

#include "StringCompareOptions.h"

namespace Native
{
	class StringUtils
	{
	public:

		StringUtils() = delete; // static class

		static bool Equals(const std::string_view str1, const std::string_view str2, StringCompareOptions options = StringCompareOptions::None);
		static bool Equals(const std::wstring_view str1, const std::wstring_view str2, StringCompareOptions options = StringCompareOptions::None);

		static bool StartsWith(const std::string_view str, const std::string_view prefix, StringCompareOptions options = StringCompareOptions::None);
		static bool StartsWith(const std::wstring_view str, const std::wstring_view prefix, StringCompareOptions options = StringCompareOptions::None);
	};
}