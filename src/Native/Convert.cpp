#include "Convert.h"

#include <optional>

namespace Native
{
	template<>
	DoubleChar<char> Convert::ToHex(const std::byte value)
	{
		constexpr auto toHexDigit = [](const unsigned char digit) constexpr -> char {
			return digit < 10 ? '0' + digit : 'A' + (digit - 10);
		};

		return DoubleChar<char>{ toHexDigit(static_cast<unsigned char>(value) >> 4), toHexDigit(static_cast<unsigned char>(value) & 0x0F) };
	}

	template<>
	DoubleChar<wchar_t> Convert::ToHex(const std::byte value)
	{
		constexpr auto toHexDigit = [](const unsigned char digit) constexpr -> wchar_t {
			return digit < 10 ? L'0' + digit : L'A' + (digit - 10);
		};

		return DoubleChar<wchar_t>{ toHexDigit(static_cast<unsigned char>(value >> 4)), toHexDigit(static_cast<unsigned char>(value) & 0x0F) };
	}

	template<typename TChar>
		requires std::same_as<TChar, char> || std::same_as<TChar, wchar_t>
	 std::optional<std::byte> FromSingleHex(const TChar single_char)
	{
		if (single_char >= '0' && single_char <= '9')
			return static_cast<std::byte>(single_char - '0');
		if (single_char >= 'A' && single_char <= 'F')
			return static_cast<std::byte>(single_char - 'A' + 10);
		if (single_char >= 'a' && single_char <= 'f')
			return static_cast<std::byte>(single_char - 'a' + 10);

		return std::nullopt;
	}

	bool Convert::FromHex(const DoubleChar<char> chars, std::byte& out_value)
	{
		const auto high = FromSingleHex<char>(chars.Char1);

		if (!high.has_value())
			return false;

		const auto low = FromSingleHex<char>(chars.Char2);

		if (!low.has_value())
			return false;

		out_value = (*high << 4) | *low;

		return true;
	}

	bool Convert::FromHex(const DoubleChar<wchar_t> chars, std::byte& out_value)
	{
		const auto high = FromSingleHex<wchar_t>(chars.Char1);

		if (!high.has_value())
			return false;

		const auto low = FromSingleHex<wchar_t>(chars.Char2);

		if (!low.has_value())
			return false;

		out_value = (*high << 4) | *low;

		return true;
	}
}