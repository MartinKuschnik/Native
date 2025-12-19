#include <benchmark/benchmark.h>

#include "Lazy.h"

static void Lazy_CreateNotThreadSafe(benchmark::State& state) {
	for (auto _ : state)
	{
		Native::Lazy<int> lazy([]() { return 42; }, false);

		benchmark::DoNotOptimize(lazy);
	}
}

BENCHMARK(Lazy_CreateNotThreadSafe);

static void Lazy_CreateThreadSafe(benchmark::State& state) {
	for (auto _ : state)
	{
		Native::Lazy<int> lazy([]() { return 42; }, true);

		benchmark::DoNotOptimize(lazy);
	}
}

BENCHMARK(Lazy_CreateThreadSafe);

static void Lazy_CreatePublicationOnly(benchmark::State& state) {
	for (auto _ : state)
	{
		Native::Lazy<int> lazy([]() { return 42; }, Native::Threading::LazyThreadSafetyMode::PublicationOnly);

		benchmark::DoNotOptimize(lazy);
	}
}

BENCHMARK(Lazy_CreatePublicationOnly);

static void Lazy_GetValueNotThreadSafeFirstTime(benchmark::State& state) {
	for (auto _ : state)
	{
		state.PauseTiming();
		Native::Lazy<int> lazy([]() { return 42; }, false);
		state.ResumeTiming();

		const int value = lazy.value();

		benchmark::DoNotOptimize(value);
	}
}

BENCHMARK(Lazy_GetValueNotThreadSafeFirstTime);

static void Lazy_GetValueNotThreadSafeSecondTime(benchmark::State& state) {
	Native::Lazy<int> lazy([]() { return 42; }, false);
	lazy.value();

	for (auto _ : state)
	{
		const int value = lazy.value();

		benchmark::DoNotOptimize(value);
	}
}

BENCHMARK(Lazy_GetValueNotThreadSafeSecondTime);

static void Lazy_GetValueThreadSafeFirstTime(benchmark::State& state) {
	for (auto _ : state)
	{
		state.PauseTiming();
		Native::Lazy<int> lazy([]() { return 42; }, true);
		state.ResumeTiming();

		const int value = lazy.value();

		benchmark::DoNotOptimize(value);
	}
}

BENCHMARK(Lazy_GetValueThreadSafeFirstTime);

static void Lazy_GetValueThreadSafeSecondTime(benchmark::State& state) {
	Native::Lazy<int> lazy([]() { return 42; }, true);
	lazy.value();

	for (auto _ : state)
	{
		const int value = lazy.value();

		benchmark::DoNotOptimize(value);
	}
}

BENCHMARK(Lazy_GetValueThreadSafeSecondTime);

static void Lazy_GetValuePublicationOnlyFirstTime(benchmark::State& state) {
	for (auto _ : state)
	{
		state.PauseTiming();
		Native::Lazy<int> lazy([]() { return 42; }, Native::Threading::LazyThreadSafetyMode::PublicationOnly);
		state.ResumeTiming();

		const int value = lazy.value();

		benchmark::DoNotOptimize(value);
	}
}

BENCHMARK(Lazy_GetValuePublicationOnlyFirstTime);

static void Lazy_GetValuePublicationOnlySecondTime(benchmark::State& state) {
	Native::Lazy<int> lazy([]() { return 42; }, Native::Threading::LazyThreadSafetyMode::PublicationOnly);
	lazy.value();

	for (auto _ : state)
	{
		const int value = lazy.value();

		benchmark::DoNotOptimize(value);
	}
}

BENCHMARK(Lazy_GetValuePublicationOnlySecondTime);

static void Lazy_GetValueWithExpensiveComputation(benchmark::State& state) {
	for (auto _ : state)
	{
		state.PauseTiming();
		Native::Lazy<int> lazy([]() {
			int result = 0;
			for (int i = 0; i < 100; ++i)
				result += i;
			return result;
		}, false);
		state.ResumeTiming();

		const int value = lazy.value();

		benchmark::DoNotOptimize(value);
	}
}

BENCHMARK(Lazy_GetValueWithExpensiveComputation);

static void Lazy_OperatorArrowNotThreadSafe(benchmark::State& state) {
	Native::Lazy<std::string> lazy([]() { return std::string("Hello World"); }, false);
	lazy.value();

	for (auto _ : state)
	{
		const size_t length = lazy->length();

		benchmark::DoNotOptimize(length);
	}
}

BENCHMARK(Lazy_OperatorArrowNotThreadSafe);

static void Lazy_OperatorStarNotThreadSafe(benchmark::State& state) {
	Native::Lazy<int> lazy([]() { return 42; }, false);
	lazy.value();

	for (auto _ : state)
	{
		const int value = *lazy;

		benchmark::DoNotOptimize(value);
	}
}

BENCHMARK(Lazy_OperatorStarNotThreadSafe);

static void Lazy_GetValueStringNotThreadSafe(benchmark::State& state) {
	Native::Lazy<std::string> lazy([]() { return std::string("Hello World"); }, false);
	lazy.value();

	for (auto _ : state)
	{
		const std::string& value = lazy.value();

		benchmark::DoNotOptimize(value);
	}
}

BENCHMARK(Lazy_GetValueStringNotThreadSafe);

static void Lazy_GetValueStringThreadSafe(benchmark::State& state) {
	Native::Lazy<std::string> lazy([]() { return std::string("Hello World"); }, true);
	lazy.value();

	for (auto _ : state)
	{
		const std::string& value = lazy.value();

		benchmark::DoNotOptimize(value);
	}
}

BENCHMARK(Lazy_GetValueStringThreadSafe);
