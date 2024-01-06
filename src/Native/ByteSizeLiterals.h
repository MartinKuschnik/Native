#pragma once

#include <cstddef>

namespace Native
{
	namespace ByteSizeLiterals
	{
		constexpr uint64_t operator""_B(const uint64_t x) {
			return static_cast<uint64_t>(x);
		}

		constexpr uint64_t operator""_kB(const uint64_t x) {
			return 1000ULL * x;
		}

		constexpr uint64_t operator""_MB(const uint64_t x) {
			return 1000_kB * x;
		}

		constexpr uint64_t operator""_GB(const uint64_t x) {
			return 1000_MB * x;
		}

		constexpr uint64_t operator""_TB(const uint64_t x) {
			return 1000_GB * x;
		}

		constexpr uint64_t operator""_PB(const uint64_t x) {
			return 1000_TB * x;
		}

		constexpr uint64_t operator""_EB(const uint64_t x) {
			return 1000_PB * x;
		}

		constexpr uint64_t operator""_ZB(const uint64_t x) {
			return 1000_EB * x;
		}

		constexpr uint64_t operator""_YB(const uint64_t x) {
			return 1000_ZB * x;
		}

		constexpr uint64_t operator""_KiB(const uint64_t x) {
			return 1024ULL * x;
		}

		constexpr uint64_t operator""_MiB(const uint64_t x) {
			return 1024_KiB * x;
		}

		constexpr uint64_t operator""_GiB(const uint64_t x) {
			return 1024_MiB * x;
		}

		constexpr uint64_t operator""_TiB(const uint64_t x) {
			return 1024_GiB * x;
		}

		constexpr uint64_t operator""_PiB(const uint64_t x) {
			return 1024_TiB * x;
		}

		constexpr uint64_t operator""_EiB(const uint64_t x) {
			return 1024_PiB * x;
		}

		constexpr uint64_t operator""_ZiB(const uint64_t x) {
			return 1024_EiB * x;
		}

		constexpr uint64_t operator""_YiB(const uint64_t x) {
			return 1024_ZiB * x;
		}
	}
}