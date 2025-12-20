#include <benchmark/benchmark.h>

#include <thread>

#include "Guid.h"

static void Guid_Constexpr(benchmark::State& state) {
	for (auto _ : state)
	{
		constexpr Native::Guid guid{ 0x122AACBB, 0x8F4B, 0x486C , 0x8C13, 0x9330A0679126 };

		benchmark::DoNotOptimize(guid);
	}
}

BENCHMARK(Guid_Constexpr);

static void Guid_ParseWithBrackets(benchmark::State& state) {
	for (auto _ : state)
	{
		const Native::Guid guid = Native::Guid::Parse("{122AACBB-8F4B-486C-8C13-9330A0679126}");

		std::this_thread::sleep_for(std::chrono::seconds(1));

		benchmark::DoNotOptimize(guid);
	}
}

BENCHMARK(Guid_ParseWithBrackets);

static void Guid_ParseWithBracketsW(benchmark::State& state) {
	for (auto _ : state)
	{
		const Native::Guid guid = Native::Guid::Parse(L"{122AACBB-8F4B-486C-8C13-9330A0679126}");

		std::this_thread::sleep_for(std::chrono::seconds(1));

		benchmark::DoNotOptimize(guid);
	}
}

BENCHMARK(Guid_ParseWithBracketsW);

static void Guid_ParseWithoutBrackets(benchmark::State& state) {
	for (auto _ : state)
	{
		const Native::Guid guid = Native::Guid::Parse("122AACBB-8F4B-486C-8C13-9330A0679126");

		benchmark::DoNotOptimize(guid);
	}
}

BENCHMARK(Guid_ParseWithoutBrackets);

static void Guid_ParseWithoutBracketsW(benchmark::State& state) {
	for (auto _ : state)
	{
		const Native::Guid guid = Native::Guid::Parse(L"122AACBB-8F4B-486C-8C13-9330A0679126");

		benchmark::DoNotOptimize(guid);
	}
}

BENCHMARK(Guid_ParseWithoutBracketsW);

static void Guid_TryParseNoneWithoutBrackets(benchmark::State& state) {
	for (auto _ : state)
	{
		const auto guid = Native::Guid::TryParse("9330A0679126");

		benchmark::DoNotOptimize(guid);
	}
}

BENCHMARK(Guid_TryParseNoneWithoutBrackets);

static void Guid_TryParseNoneWithoutBracketsW(benchmark::State& state) {
	for (auto _ : state)
	{
		const auto guid = Native::Guid::TryParse(L"9330A0679126");

		benchmark::DoNotOptimize(guid);
	}
}

BENCHMARK(Guid_TryParseNoneWithoutBracketsW);

static void Guid_ParseEmptyWithoutBrackets(benchmark::State& state) {
	for (auto _ : state)
	{
		const auto guid = Native::Guid::TryParse("00000000-0000-0000-0000-000000000000");

		benchmark::DoNotOptimize(guid);
	}
}

BENCHMARK(Guid_ParseEmptyWithoutBrackets);

static void Guid_ParseEmptyWithoutBracketsW(benchmark::State& state) {
	for (auto _ : state)
	{
		const auto guid = Native::Guid::TryParse(L"00000000-0000-0000-0000-000000000000");

		benchmark::DoNotOptimize(guid);
	}
}

BENCHMARK(Guid_ParseEmptyWithoutBracketsW);

static void Guid_ToString(benchmark::State& state)
{
	constexpr const Native::Guid guid { 0xe11b7641, 0x3a9b, 0x4d9e,  0x9cb7, 0x72d85b0d81fd  };

	for (auto _ : state)
	{
		const std::string guid_as_string = guid.string();

		benchmark::DoNotOptimize(guid_as_string);
	}
}

BENCHMARK(Guid_ToString);

static void Guid_ToWString(benchmark::State& state)
{
	constexpr const Native::Guid guid{ 0xe11b7641, 0x3a9b, 0x4d9e,  0x9cb7, 0x72d85b0d81fd };

	for (auto _ : state)
	{
		const std::wstring guid_as_string = guid.wstring();

		benchmark::DoNotOptimize(guid_as_string);
	}
}

BENCHMARK(Guid_ToWString);