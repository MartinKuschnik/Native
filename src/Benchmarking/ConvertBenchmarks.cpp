#include <benchmark/benchmark.h>

#include "Convert.h"

static void Convert_ToHexChar(benchmark::State& state) {
	constexpr std::byte value{ 0xAB };

	for (auto _ : state)
	{
		const Native::DoubleChar<char> hex = Native::Convert::ToHex<char>(value);

		benchmark::DoNotOptimize(hex);
	}
}

BENCHMARK(Convert_ToHexChar);

static void Convert_ToHexWChar(benchmark::State& state) {
	constexpr std::byte value{ 0xAB };

	for (auto _ : state)
	{
		const Native::DoubleChar<wchar_t> hex = Native::Convert::ToHex<wchar_t>(value);

		benchmark::DoNotOptimize(hex);
	}
}

BENCHMARK(Convert_ToHexWChar);

static void Convert_ToHexCharMinValue(benchmark::State& state) {
	constexpr std::byte value{ 0x00 };

	for (auto _ : state)
	{
		const Native::DoubleChar<char> hex = Native::Convert::ToHex<char>(value);

		benchmark::DoNotOptimize(hex);
	}
}

BENCHMARK(Convert_ToHexCharMinValue);

static void Convert_ToHexCharMaxValue(benchmark::State& state) {
	constexpr std::byte value{ 0xFF };

	for (auto _ : state)
	{
		const Native::DoubleChar<char> hex = Native::Convert::ToHex<char>(value);

		benchmark::DoNotOptimize(hex);
	}
}

BENCHMARK(Convert_ToHexCharMaxValue);

static void Convert_FromHexCharUpperCase(benchmark::State& state) {
	constexpr Native::DoubleChar<char> chars{ 'A', 'B' };

	for (auto _ : state)
	{
		std::byte value;
		const bool success = Native::Convert::FromHex(chars, value);

		benchmark::DoNotOptimize(value);
		benchmark::DoNotOptimize(success);
	}
}

BENCHMARK(Convert_FromHexCharUpperCase);

static void Convert_FromHexCharLowerCase(benchmark::State& state) {
	constexpr Native::DoubleChar<char> chars{ 'a', 'b' };

	for (auto _ : state)
	{
		std::byte value;
		const bool success = Native::Convert::FromHex(chars, value);

		benchmark::DoNotOptimize(value);
		benchmark::DoNotOptimize(success);
	}
}

BENCHMARK(Convert_FromHexCharLowerCase);

static void Convert_FromHexCharDigits(benchmark::State& state) {
	constexpr Native::DoubleChar<char> chars{ '1', '2' };

	for (auto _ : state)
	{
		std::byte value;
		const bool success = Native::Convert::FromHex(chars, value);

		benchmark::DoNotOptimize(value);
		benchmark::DoNotOptimize(success);
	}
}

BENCHMARK(Convert_FromHexCharDigits);

static void Convert_FromHexCharMixed(benchmark::State& state) {
	constexpr Native::DoubleChar<char> chars{ '8', 'C' };

	for (auto _ : state)
	{
		std::byte value;
		const bool success = Native::Convert::FromHex(chars, value);

		benchmark::DoNotOptimize(value);
		benchmark::DoNotOptimize(success);
	}
}

BENCHMARK(Convert_FromHexCharMixed);

static void Convert_FromHexCharZeros(benchmark::State& state) {
	constexpr Native::DoubleChar<char> chars{ '0', '0' };

	for (auto _ : state)
	{
		std::byte value;
		const bool success = Native::Convert::FromHex(chars, value);

		benchmark::DoNotOptimize(value);
		benchmark::DoNotOptimize(success);
	}
}

BENCHMARK(Convert_FromHexCharZeros);

static void Convert_FromHexCharMaxValue(benchmark::State& state) {
	constexpr Native::DoubleChar<char> chars{ 'F', 'F' };

	for (auto _ : state)
	{
		std::byte value;
		const bool success = Native::Convert::FromHex(chars, value);

		benchmark::DoNotOptimize(value);
		benchmark::DoNotOptimize(success);
	}
}

BENCHMARK(Convert_FromHexCharMaxValue);

static void Convert_FromHexCharInvalid(benchmark::State& state) {
	constexpr Native::DoubleChar<char> chars{ 'G', 'Z' };

	for (auto _ : state)
	{
		std::byte value;
		const bool success = Native::Convert::FromHex(chars, value);

		benchmark::DoNotOptimize(value);
		benchmark::DoNotOptimize(success);
	}
}

BENCHMARK(Convert_FromHexCharInvalid);

static void Convert_FromHexWCharUpperCase(benchmark::State& state) {
	constexpr Native::DoubleChar<wchar_t> chars{ L'A', L'B' };

	for (auto _ : state)
	{
		std::byte value;
		const bool success = Native::Convert::FromHex(chars, value);

		benchmark::DoNotOptimize(value);
		benchmark::DoNotOptimize(success);
	}
}

BENCHMARK(Convert_FromHexWCharUpperCase);

static void Convert_FromHexWCharLowerCase(benchmark::State& state) {
	constexpr Native::DoubleChar<wchar_t> chars{ L'a', L'b' };

	for (auto _ : state)
	{
		std::byte value;
		const bool success = Native::Convert::FromHex(chars, value);

		benchmark::DoNotOptimize(value);
		benchmark::DoNotOptimize(success);
	}
}

BENCHMARK(Convert_FromHexWCharLowerCase);

static void Convert_FromHexWCharDigits(benchmark::State& state) {
	constexpr Native::DoubleChar<wchar_t> chars{ L'1', L'2' };

	for (auto _ : state)
	{
		std::byte value;
		const bool success = Native::Convert::FromHex(chars, value);

		benchmark::DoNotOptimize(value);
		benchmark::DoNotOptimize(success);
	}
}

BENCHMARK(Convert_FromHexWCharDigits);

static void Convert_FromHexWCharInvalid(benchmark::State& state) {
	constexpr Native::DoubleChar<wchar_t> chars{ L'G', L'Z' };

	for (auto _ : state)
	{
		std::byte value;
		const bool success = Native::Convert::FromHex(chars, value);

		benchmark::DoNotOptimize(value);
		benchmark::DoNotOptimize(success);
	}
}

BENCHMARK(Convert_FromHexWCharInvalid);

static void Convert_RoundTripChar(benchmark::State& state) {
	constexpr std::byte original_value{ 0xCD };

	for (auto _ : state)
	{
		const Native::DoubleChar<char> hex = Native::Convert::ToHex<char>(original_value);
		
		std::byte value;
		const bool success = Native::Convert::FromHex(hex, value);

		benchmark::DoNotOptimize(hex);
		benchmark::DoNotOptimize(value);
		benchmark::DoNotOptimize(success);
	}
}

BENCHMARK(Convert_RoundTripChar);

static void Convert_RoundTripWChar(benchmark::State& state) {
	constexpr std::byte original_value{ 0xCD };

	for (auto _ : state)
	{
		const Native::DoubleChar<wchar_t> hex = Native::Convert::ToHex<wchar_t>(original_value);
		
		std::byte value;
		const bool success = Native::Convert::FromHex(hex, value);

		benchmark::DoNotOptimize(hex);
		benchmark::DoNotOptimize(value);
		benchmark::DoNotOptimize(success);
	}
}

BENCHMARK(Convert_RoundTripWChar);
