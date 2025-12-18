#pragma once

#include <concepts>

namespace Native
{
	/// <summary>	
	/// Represents a pair of two characters of the same type.
	/// </summary>
	/// <typeparam name="TChar">The character type, must be either char or wchar_t.</typeparam>
	template<typename TChar>
		requires std::same_as<TChar, char> || std::same_as<TChar, wchar_t>
	struct DoubleChar
	{
		TChar Char1;
		TChar Char2;
	};
}