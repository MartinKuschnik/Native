#include <benchmark/benchmark.h>

#include "StringUtils.h"

static void StringUtils_EqualsShortStringSame(benchmark::State& state) {
	const std::string str1 = "Hello";
	const std::string str2 = "Hello";

	for (auto _ : state)
	{
		const bool result = Native::StringUtils::Equals(str1, str2);

		benchmark::DoNotOptimize(result);
	}
}

BENCHMARK(StringUtils_EqualsShortStringSame);

static void StringUtils_EqualsShortStringDifferent(benchmark::State& state) {
	const std::string str1 = "Hello";
	const std::string str2 = "World";

	for (auto _ : state)
	{
		const bool result = Native::StringUtils::Equals(str1, str2);

		benchmark::DoNotOptimize(result);
	}
}

BENCHMARK(StringUtils_EqualsShortStringDifferent);

static void StringUtils_EqualsLongStringSame(benchmark::State& state) {
	const std::string str1 = "This is a very long string that we use for benchmarking purposes";
	const std::string str2 = "This is a very long string that we use for benchmarking purposes";

	for (auto _ : state)
	{
		const bool result = Native::StringUtils::Equals(str1, str2);

		benchmark::DoNotOptimize(result);
	}
}

BENCHMARK(StringUtils_EqualsLongStringSame);

static void StringUtils_EqualsLongStringDifferent(benchmark::State& state) {
	const std::string str1 = "This is a very long string that we use for benchmarking purposes";
	const std::string str2 = "This is a very long string that we use for different purposes";

	for (auto _ : state)
	{
		const bool result = Native::StringUtils::Equals(str1, str2);

		benchmark::DoNotOptimize(result);
	}
}

BENCHMARK(StringUtils_EqualsLongStringDifferent);

static void StringUtils_EqualsIgnoreCase(benchmark::State& state) {
	const std::string str1 = "Hello";
	const std::string str2 = "HELLO";

	for (auto _ : state)
	{
		const bool result = Native::StringUtils::Equals(str1, str2, Native::StringCompareOptions::IgnoreCase);

		benchmark::DoNotOptimize(result);
	}
}

BENCHMARK(StringUtils_EqualsIgnoreCase);

static void StringUtils_EqualsIgnoreCaseDifferent(benchmark::State& state) {
	const std::string str1 = "Hello";
	const std::string str2 = "WORLD";

	for (auto _ : state)
	{
		const bool result = Native::StringUtils::Equals(str1, str2, Native::StringCompareOptions::IgnoreCase);

		benchmark::DoNotOptimize(result);
	}
}

BENCHMARK(StringUtils_EqualsIgnoreCaseDifferent);

static void StringUtils_EqualsWStringSame(benchmark::State& state) {
	const std::wstring str1 = L"Hello";
	const std::wstring str2 = L"Hello";

	for (auto _ : state)
	{
		const bool result = Native::StringUtils::Equals(str1, str2);

		benchmark::DoNotOptimize(result);
	}
}

BENCHMARK(StringUtils_EqualsWStringSame);

static void StringUtils_EqualsWStringDifferent(benchmark::State& state) {
	const std::wstring str1 = L"Hello";
	const std::wstring str2 = L"World";

	for (auto _ : state)
	{
		const bool result = Native::StringUtils::Equals(str1, str2);

		benchmark::DoNotOptimize(result);
	}
}

BENCHMARK(StringUtils_EqualsWStringDifferent);

static void StringUtils_StartsWithTrue(benchmark::State& state) {
	const std::string str = "Hello World";
	const std::string prefix = "Hello";

	for (auto _ : state)
	{
		const bool result = Native::StringUtils::StartsWith(str, prefix);

		benchmark::DoNotOptimize(result);
	}
}

BENCHMARK(StringUtils_StartsWithTrue);

static void StringUtils_StartsWithFalse(benchmark::State& state) {
	const std::string str = "Hello World";
	const std::string prefix = "World";

	for (auto _ : state)
	{
		const bool result = Native::StringUtils::StartsWith(str, prefix);

		benchmark::DoNotOptimize(result);
	}
}

BENCHMARK(StringUtils_StartsWithFalse);

static void StringUtils_StartsWithIgnoreCaseTrue(benchmark::State& state) {
	const std::string str = "Hello World";
	const std::string prefix = "HELLO";

	for (auto _ : state)
	{
		const bool result = Native::StringUtils::StartsWith(str, prefix, Native::StringCompareOptions::IgnoreCase);

		benchmark::DoNotOptimize(result);
	}
}

BENCHMARK(StringUtils_StartsWithIgnoreCaseTrue);

static void StringUtils_StartsWithIgnoreCaseFalse(benchmark::State& state) {
	const std::string str = "Hello World";
	const std::string prefix = "WORLD";

	for (auto _ : state)
	{
		const bool result = Native::StringUtils::StartsWith(str, prefix, Native::StringCompareOptions::IgnoreCase);

		benchmark::DoNotOptimize(result);
	}
}

BENCHMARK(StringUtils_StartsWithIgnoreCaseFalse);

static void StringUtils_StartsWithWStringTrue(benchmark::State& state) {
	const std::wstring str = L"Hello World";
	const std::wstring prefix = L"Hello";

	for (auto _ : state)
	{
		const bool result = Native::StringUtils::StartsWith(str, prefix);

		benchmark::DoNotOptimize(result);
	}
}

BENCHMARK(StringUtils_StartsWithWStringTrue);

static void StringUtils_StartsWithWStringFalse(benchmark::State& state) {
	const std::wstring str = L"Hello World";
	const std::wstring prefix = L"World";

	for (auto _ : state)
	{
		const bool result = Native::StringUtils::StartsWith(str, prefix);

		benchmark::DoNotOptimize(result);
	}
}

BENCHMARK(StringUtils_StartsWithWStringFalse);

static void StringUtils_StartsWithEmptyPrefix(benchmark::State& state) {
	const std::string str = "Hello World";
	const std::string prefix = "";

	for (auto _ : state)
	{
		const bool result = Native::StringUtils::StartsWith(str, prefix);

		benchmark::DoNotOptimize(result);
	}
}

BENCHMARK(StringUtils_StartsWithEmptyPrefix);

static void StringUtils_StartsWithLongerPrefix(benchmark::State& state) {
	const std::string str = "Hello";
	const std::string prefix = "Hello World";

	for (auto _ : state)
	{
		const bool result = Native::StringUtils::StartsWith(str, prefix);

		benchmark::DoNotOptimize(result);
	}
}

BENCHMARK(StringUtils_StartsWithLongerPrefix);
