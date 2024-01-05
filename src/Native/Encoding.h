#pragma once

#include <string>

namespace Native
{
	struct Encoding
	{
		// static class
		Encoding() = delete;

		struct UTF8
		{
			// static class
			UTF8() = delete;

			/// <summary>
			/// Convert a wide string to an UTF8 string.
			/// </summary>
			/// <param name="wstr">The wide string to convert into UTF8.</param>
			/// <returns>The converted UTF8 string</returns>
			static std::string Encode(const std::wstring_view wstr);

			/// <summary>
			/// Convert a wide Unicode string to an UTF8 string.
			/// </summary>
			/// <param name="str">The UTF8 encoded string to convert into wide string.</param>
			/// <returns>The converted wide string.</returns>
			static std::wstring Decode(const std::string_view str);
		};
	};
}

