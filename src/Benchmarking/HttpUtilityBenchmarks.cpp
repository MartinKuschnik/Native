#include <benchmark/benchmark.h>

#include "HttpUtility.h"

static void HttpUtility_UrlDecode_NoEncoding(benchmark::State& state) {
	for (auto _ : state)
	{
		const std::wstring decoded = Native::Web::HttpUtility::UrlDecode(L"HelloWorld");

		benchmark::DoNotOptimize(decoded);
	}
}

BENCHMARK(HttpUtility_UrlDecode_NoEncoding);

static void HttpUtility_UrlDecode_WithSpaces(benchmark::State& state) {
	for (auto _ : state)
	{
		const std::wstring decoded = Native::Web::HttpUtility::UrlDecode(L"Hello+World+Test");

		benchmark::DoNotOptimize(decoded);
	}
}

BENCHMARK(HttpUtility_UrlDecode_WithSpaces);

static void HttpUtility_UrlDecode_WithPercentEncoding(benchmark::State& state) {
	for (auto _ : state)
	{
		const std::wstring decoded = Native::Web::HttpUtility::UrlDecode(L"Hello%20World%20Test");

		benchmark::DoNotOptimize(decoded);
	}
}

BENCHMARK(HttpUtility_UrlDecode_WithPercentEncoding);

static void HttpUtility_UrlDecode_MixedEncoding(benchmark::State& state) {
	for (auto _ : state)
	{
		const std::wstring decoded = Native::Web::HttpUtility::UrlDecode(L"Hello+World%20Test%21");

		benchmark::DoNotOptimize(decoded);
	}
}

BENCHMARK(HttpUtility_UrlDecode_MixedEncoding);

static void HttpUtility_UrlDecode_SpecialCharacters(benchmark::State& state) {
	for (auto _ : state)
	{
		const std::wstring decoded = Native::Web::HttpUtility::UrlDecode(L"name%3DJohn%26age%3D30%26city%3DNew%2BYork");

		benchmark::DoNotOptimize(decoded);
	}
}

BENCHMARK(HttpUtility_UrlDecode_SpecialCharacters);

static void HttpUtility_UrlDecode_LongUrl(benchmark::State& state) {
	for (auto _ : state)
	{
		const std::wstring decoded = Native::Web::HttpUtility::UrlDecode(
			L"https%3A%2F%2Fexample.com%2Fpath%2Fto%2Fresource%3Fparam1%3Dvalue1%26param2%3Dvalue2%26param3%3Dvalue3"
		);

		benchmark::DoNotOptimize(decoded);
	}
}

BENCHMARK(HttpUtility_UrlDecode_LongUrl);

static void HttpUtility_UrlDecode_EmptyString(benchmark::State& state) {
	for (auto _ : state)
	{
		const std::wstring decoded = Native::Web::HttpUtility::UrlDecode(L"");

		benchmark::DoNotOptimize(decoded);
	}
}

BENCHMARK(HttpUtility_UrlDecode_EmptyString);

static void HttpUtility_UrlDecode_OnlyEncodedCharacters(benchmark::State& state) {
	for (auto _ : state)
	{
		const std::wstring decoded = Native::Web::HttpUtility::UrlDecode(L"%48%65%6C%6C%6F");

		benchmark::DoNotOptimize(decoded);
	}
}

BENCHMARK(HttpUtility_UrlDecode_OnlyEncodedCharacters);

static void HttpUtility_UrlDecode_InvalidHexDigits(benchmark::State& state) {
	for (auto _ : state)
	{
		const std::wstring decoded = Native::Web::HttpUtility::UrlDecode(L"Hello%ZZWorld");

		benchmark::DoNotOptimize(decoded);
	}
}

BENCHMARK(HttpUtility_UrlDecode_InvalidHexDigits);

static void HttpUtility_UrlDecode_IncompleteEncoding(benchmark::State& state) {
	for (auto _ : state)
	{
		const std::wstring decoded = Native::Web::HttpUtility::UrlDecode(L"Hello%2World");

		benchmark::DoNotOptimize(decoded);
	}
}

BENCHMARK(HttpUtility_UrlDecode_IncompleteEncoding);

static void HttpUtility_UrlDecode_LowercaseHex(benchmark::State& state) {
	for (auto _ : state)
	{
		const std::wstring decoded = Native::Web::HttpUtility::UrlDecode(L"test%3a%2f%2fexample%2ecom");

		benchmark::DoNotOptimize(decoded);
	}
}

BENCHMARK(HttpUtility_UrlDecode_LowercaseHex);

static void HttpUtility_UrlDecode_UppercaseHex(benchmark::State& state) {
	for (auto _ : state)
	{
		const std::wstring decoded = Native::Web::HttpUtility::UrlDecode(L"test%3A%2F%2Fexample%2Ecom");

		benchmark::DoNotOptimize(decoded);
	}
}

BENCHMARK(HttpUtility_UrlDecode_UppercaseHex);

static void HttpUtility_UrlDecode_RealisticQueryString(benchmark::State& state) {
	for (auto _ : state)
	{
		const std::wstring decoded = Native::Web::HttpUtility::UrlDecode(
			L"search%3Dc%2B%2B+programming%26filter%3Drecent%26sort%3Drelevance%26page%3D1"
		);

		benchmark::DoNotOptimize(decoded);
	}
}

BENCHMARK(HttpUtility_UrlDecode_RealisticQueryString);
