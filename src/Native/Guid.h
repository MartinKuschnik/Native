#pragma once

#include <string>
#include <format>

#include "combaseapi.h"

namespace Native
{
	class Guid : public GUID
	{
	public:

		constexpr Guid() noexcept
			: GUID()
		{
		}

		constexpr Guid(const GUID& other) noexcept
			: GUID(other)
		{
		}
		constexpr Guid(const Guid& other) noexcept
			: GUID(other)
		{
		}

		const std::string string() const;
		const std::wstring wstring() const;

		operator const std::string() const;
		operator const std::wstring() const;

		static const Guid Empty;

		static const Guid NewGuid();

		static const Guid Parse(const std::string_view value);
		static const Guid Parse(const std::wstring_view value);
	};
}

namespace std {

	template <>
	struct hash<Native::Guid>
	{
		// ToDo: add unit tests for hash<Native::Guid>
		std::size_t operator()(const Native::Guid& guid) const
		{
			const std::uint64_t* p = reinterpret_cast<const std::uint64_t*>(&guid);
			std::hash<std::uint64_t> hash;
			return hash(p[0]) ^ hash(p[1]);
		}
	};
}

template <>
struct std::formatter<Native::Guid, char> : std::formatter<string_view, char>
{
	auto format(const Native::Guid& guid, std::format_context& ctx) const
	{
		return std::formatter<string_view>::format(guid.string(), ctx);
	}
};

template <>
struct std::formatter<Native::Guid, wchar_t> : std::formatter<wstring_view, wchar_t>
{
	auto format(const Native::Guid& guid, std::wformat_context& ctx) const
	{
		return std::formatter<wstring_view, wchar_t>::format(guid.wstring(), ctx);
	}
};