#pragma once

#include <cstddef>

namespace Native
{
	namespace ByteSizeLiterals
	{
		constexpr std::size_t operator""_B(unsigned long long int x) {
			return static_cast<std::size_t>(x);
		}

		constexpr std::size_t operator""_kB(unsigned long long int x) {
			return 1000ULL * x;
		}

		constexpr std::size_t operator""_MB(unsigned long long int x) {
			return 1000_kB * x;
		}

		constexpr std::size_t operator""_GB(unsigned long long int x) {
			return 1000_MB * x;
		}

		constexpr std::size_t operator""_TB(unsigned long long int x) {
			return 1000_GB * x;
		}

		constexpr std::size_t operator""_PB(unsigned long long int x) {
			return 1000_TB * x;
		}

		constexpr std::size_t operator""_EB(unsigned long long int x) {
			return 1000_PB * x;
		}

		constexpr std::size_t operator""_ZB(unsigned long long int x) {
			return 1000_EB * x;
		}

		constexpr std::size_t operator""_YB(unsigned long long int x) {
			return 1000_ZB * x;
		}

		constexpr std::size_t operator""_KiB(unsigned long long int x) {
			return 1024ULL * x;
		}

		constexpr std::size_t operator""_MiB(unsigned long long int x) {
			return 1024_KiB * x;
		}

		constexpr std::size_t operator""_GiB(unsigned long long int x) {
			return 1024_MiB * x;
		}

		constexpr std::size_t operator""_TiB(unsigned long long int x) {
			return 1024_GiB * x;
		}

		constexpr std::size_t operator""_PiB(unsigned long long int x) {
			return 1024_TiB * x;
		}

		constexpr std::size_t operator""_EiB(unsigned long long int x) {
			return 1024_PiB * x;
		}

		constexpr std::size_t operator""_ZiB(unsigned long long int x) {
			return 1024_EiB * x;
		}

		constexpr std::size_t operator""_YiB(unsigned long long int x) {
			return 1024_ZiB * x;
		}
	}
}