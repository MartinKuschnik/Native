#pragma once

#include <concepts>

#include "DoubleChar.h"

namespace Native
{
	struct Convert
	{
		/// <summary>
		/// Converts a byte value to its hexadecimal representation as two characters.
		/// </summary>
		/// <typeparam name="TChar">The character type, must be either char or wchar_t.</typeparam>
		/// <param name="value">The byte value to convert.</param>
		/// <returns>A DoubleChar containing the two hexadecimal characters representing the byte.</returns>
		template<typename TChar>
			requires std::same_as<TChar, char> || std::same_as<TChar, wchar_t>
		[[nodiscard]]
		static DoubleChar<TChar> ToHex(const std::byte value);

		/// <summary>
		/// Converts two hexadecimal characters to a byte value.
		/// </summary>
		/// <param name="chars">The two hexadecimal characters to convert.</param>
		/// <param name="out_value">The resulting byte value if conversion succeeds.</param>
		/// <returns>True if the conversion was successful, false otherwise.</returns>
		[[nodiscard]]
		static bool FromHex(const DoubleChar<char> chars, std::byte& out_value);

		/// <summary>
		/// Converts two hexadecimal wide characters to a byte value.
		/// </summary>
		/// <param name="chars">The two hexadecimal wide characters to convert.</param>
		/// <param name="out_value">The resulting byte value if conversion succeeds.</param>
		/// <returns>True if the conversion was successful, false otherwise.</returns>
		[[nodiscard]]
		static bool FromHex(const DoubleChar<wchar_t> chars, std::byte& out_value);

	private:
		// static class
		Convert() = delete;
	};

	template<>
	DoubleChar<char> Convert::ToHex(const std::byte value);

	template<>
	DoubleChar<wchar_t> Convert::ToHex(const std::byte value);
}