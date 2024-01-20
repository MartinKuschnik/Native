#pragma once

#include <cstddef>

#include <format>

#include <shlwapi.h>

#include <Exception.h>
#include <Util.h>

#pragma comment(lib, "Shlwapi.lib")

namespace Native
{
	class ByteSize
	{
	public:
		constexpr ByteSize() noexcept
			: _value(0)
		{
		}

		constexpr ByteSize(const uint64_t value) noexcept
			: _value(value)
		{
		}

		constexpr operator uint64_t () const
		{
			return this->_value;
		}

		constexpr bool operator == (const ByteSize& other)
		{
			return this->_value == other._value;
		}

		constexpr bool operator != (const ByteSize& other)
		{
			return this->_value != other._value;
		}

		constexpr bool operator < (const ByteSize& other)
		{
			return this->_value < other._value;
		}

		constexpr bool operator <= (const ByteSize& other)
		{
			return this->_value <= other._value;
		}

		constexpr bool operator > (const ByteSize& other)
		{
			return this->_value > other._value;
		}

		constexpr bool operator >= (const ByteSize& other)
		{
			return this->_value >= other._value;
		}

	private:
		uint64_t _value;
	};

	namespace ByteSizeLiterals
	{
		constexpr ByteSize operator""_B(const uint64_t x) {
			return ByteSize(x);
		}

		constexpr ByteSize operator""_KB(const uint64_t x) {
			return ByteSize(1024ULL * x);
		}

		constexpr ByteSize operator""_MB(const uint64_t x) {
			return ByteSize(1024_KB * x);
		}

		constexpr ByteSize operator""_GB(const uint64_t x) {
			return ByteSize(1024_MB * x);
		}

		constexpr ByteSize operator""_TB(const uint64_t x) {
			return ByteSize(1024_GB * x);
		}

		constexpr ByteSize operator""_PB(const uint64_t x) {
			return ByteSize(1024_TB * x);
		}

		constexpr ByteSize operator""_EB(const uint64_t x) {
			return ByteSize(1024_PB * x);
		}

		constexpr ByteSize operator""_ZB(const uint64_t x) {
			return ByteSize(1024_EB * x);
		}

		constexpr ByteSize operator""_YB(const uint64_t x) {
			return ByteSize(1024_ZB * x);
		}
	}
}

template <>
struct std::formatter<Native::ByteSize, char> : std::formatter<string_view, char>
{
	auto format(const Native::ByteSize& byte_size, std::format_context& ctx) const
	{
		constexpr size_t buffer_size = 40;

		char buffer[buffer_size];

		const auto result = StrFormatByteSize64A(static_cast<LONGLONG>(byte_size), buffer, buffer_size);

		if (result == 0)
			throw Native::Exception(std::format("{0} failed.", nameof(StrFormatByteSize64A)));

		return std::formatter<string_view>::format(std::string(buffer), ctx);
	}
};

template <>
struct std::formatter<Native::ByteSize, wchar_t> : std::formatter<wstring_view, wchar_t>
{
	auto format(const Native::ByteSize& byte_size, std::wformat_context& ctx) const
	{
		constexpr size_t buffer_size = 40;

		wchar_t buffer[buffer_size];

		const auto result = StrFormatByteSizeW(static_cast<LONGLONG>(byte_size), buffer, buffer_size);

		if (result == 0)
			throw Native::Exception(std::format("{0} failed.", nameof(StrFormatByteSizeW)));

		return std::formatter<wstring_view, wchar_t>::format(std::wstring(buffer), ctx);
	}
};