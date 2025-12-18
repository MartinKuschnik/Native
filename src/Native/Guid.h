#pragma once

#include <string>
#include <format>
#include <optional>

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


		constexpr Guid(uint32_t g1, uint16_t g2, uint16_t g3, uint16_t g4, uint64_t g5) noexcept
			: GUID{ g1, g2, g3, {
					static_cast<uint8_t>(g4 >> 8),
					static_cast<uint8_t>(g4),
					static_cast<uint8_t>(g5 >> 40),
					static_cast<uint8_t>(g5 >> 32),
					static_cast<uint8_t>(g5 >> 24),
					static_cast<uint8_t>(g5 >> 16),
					static_cast<uint8_t>(g5 >> 8),
					static_cast<uint8_t>(g5)
				} 
			}
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

		std::string string() const;
		std::wstring wstring() const;

		operator std::string() const;
		operator std::wstring() const;

		static const Guid Empty;

		static Guid NewGuid();

		static Guid Parse(const std::string_view value);
		static Guid Parse(const std::wstring_view value);

		static std::optional<Guid> TryParse(const std::string_view value);
		static std::optional<Guid> TryParse(const std::wstring_view value);

	};
}

namespace std {

	template <>
	struct hash<Native::Guid>
	{
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