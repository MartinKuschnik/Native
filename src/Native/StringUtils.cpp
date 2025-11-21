#include "StringUtils.h"

namespace Native
{
    bool StringUtils::Equals(const std::string_view str1, const std::string_view str2, StringCompareOptions options)
    {
        if (options == StringCompareOptions::None)
            return str1 == str2;

        if (str1.size() != str2.size())
            return false;

        return std::ranges::equal(str1, str2,
            [](const char a, const char b)
        {
            return std::tolower((unsigned char)a) ==
                std::tolower((unsigned char)b);
        });
    }

    bool Native::StringUtils::Equals(const std::wstring_view str1, const std::wstring_view str2, StringCompareOptions options)
    {
        if (options == StringCompareOptions::None)
            return str1 == str2;

        if (str1.size() != str2.size())
            return false;

        return std::ranges::equal(str1, str2,
            [](const wchar_t a, const wchar_t b)
        {
            return std::tolower(a) == std::tolower(b);
        });
    }

    bool StringUtils::StartsWith(const std::string_view str, const std::string_view prefix, StringCompareOptions options)
    {
		return StringUtils::Equals(str.substr(0, prefix.size()), prefix, options);
    }

    bool StringUtils::StartsWith(const std::wstring_view str, const std::wstring_view prefix, StringCompareOptions options)
    {
        return StringUtils::Equals(str.substr(0, prefix.size()), prefix, options);
    }
}