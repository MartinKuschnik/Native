#include <benchmark/benchmark.h>

#include "Guid.h"

static void Constexpr(benchmark::State& state) {
	for (auto _ : state)
	{
		constexpr Native::Guid guid{ 0x122AACBB, 0x8F4B, 0x486C , 0x8C13, 0x9330A0679126 };

		benchmark::DoNotOptimize(guid);
	}
}

BENCHMARK(Constexpr);

static void ParseGuidWithBrackets(benchmark::State& state) {
	for (auto _ : state)
	{
		const Native::Guid guid = Native::Guid::Parse("{122AACBB-8F4B-486C-8C13-9330A0679126}");

		std::string b("jbghjvv z ftzf hzh gjguzguzgzugzjgzuftzfgugzugftz  gz gz ggz gzug zug zj zu ");

		benchmark::DoNotOptimize(guid);
	}
}

BENCHMARK(ParseGuidWithBrackets);

static void ParseGuidWithBracketsW(benchmark::State& state) {
	for (auto _ : state)
	{
		const Native::Guid guid = Native::Guid::Parse(L"{122AACBB-8F4B-486C-8C13-9330A0679126}");

		benchmark::DoNotOptimize(guid);
	}
}

BENCHMARK(ParseGuidWithBracketsW);

static void ParseGuidWithoutBrackets(benchmark::State& state) {
	for (auto _ : state)
	{
		const Native::Guid guid = Native::Guid::Parse("122AACBB-8F4B-486C-8C13-9330A0679126");

		benchmark::DoNotOptimize(guid);
	}
}

BENCHMARK(ParseGuidWithoutBrackets);


static void ParseGuidWithoutBracketsW(benchmark::State& state) {
	for (auto _ : state)
	{
		const Native::Guid guid = Native::Guid::Parse(L"122AACBB-8F4B-486C-8C13-9330A0679126");

		benchmark::DoNotOptimize(guid);
	}
}

BENCHMARK(ParseGuidWithoutBracketsW);

static void TryParseNoneGuidWithoutBrackets(benchmark::State& state) {
	for (auto _ : state)
	{
		const auto guid = Native::Guid::TryParse("9330A0679126");

		benchmark::DoNotOptimize(guid);
	}
}

BENCHMARK(TryParseNoneGuidWithoutBrackets);

static void TryParseNoneGuidWithoutBracketsW(benchmark::State& state) {
	for (auto _ : state)
	{
		const auto guid = Native::Guid::TryParse(L"9330A0679126");

		benchmark::DoNotOptimize(guid);
	}
}

BENCHMARK(TryParseNoneGuidWithoutBracketsW);


static void ParseEmptyGuidWithoutBrackets(benchmark::State& state) {
	for (auto _ : state)
	{
		const auto guid = Native::Guid::TryParse("00000000-0000-0000-0000-000000000000");

		benchmark::DoNotOptimize(guid);
	}
}

BENCHMARK(ParseEmptyGuidWithoutBrackets);


static void ParseEmptyGuidWithoutBracketsW(benchmark::State& state) {
	for (auto _ : state)
	{
		const auto guid = Native::Guid::TryParse(L"00000000-0000-0000-0000-000000000000");

		benchmark::DoNotOptimize(guid);
	}
}

BENCHMARK(ParseEmptyGuidWithoutBracketsW);

static void GuidToString(benchmark::State& state)
{
	constexpr const Native::Guid guid { 0xe11b7641, 0x3a9b, 0x4d9e,  0x9cb7, 0x72d85b0d81fd  };

	for (auto _ : state)
	{
		const std::string guid_as_string = guid.string();

		benchmark::DoNotOptimize(guid_as_string);
	}
}

BENCHMARK(GuidToString);

static void GuidToWString(benchmark::State& state)
{
	constexpr const Native::Guid guid{ 0xe11b7641, 0x3a9b, 0x4d9e,  0x9cb7, 0x72d85b0d81fd };

	for (auto _ : state)
	{
		const std::wstring guid_as_string = guid.wstring();

		benchmark::DoNotOptimize(guid_as_string);
	}
}

BENCHMARK(GuidToWString);