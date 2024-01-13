#pragma once

#include <chrono>

#include <optional>

#include <concepts>

namespace Native
{
	namespace Diagnostics
	{
		template <class _Tp>
		struct is_chrono_duration : std::false_type {};

		template <class _Rep, class _Period>
		struct is_chrono_duration<std::chrono::duration<_Rep, _Period>> : std::true_type {};

		class Stopwatch
		{
		private:
			std::optional<std::chrono::time_point<std::chrono::steady_clock>> _start;
			std::optional<std::chrono::time_point<std::chrono::steady_clock>> _end;

			Stopwatch(const bool start) noexcept;
		public:
			Stopwatch() noexcept = default;

			void start() noexcept;
			void stop() noexcept;


			template<typename TDuration>
			requires is_chrono_duration<TDuration>::value
			TDuration elapsed() const noexcept
			{
				if (!this->_start.has_value())
					return TDuration(0);

				if (!this->_end.has_value())
					return std::chrono::duration_cast<TDuration>(std::chrono::high_resolution_clock::now() - this->_start.value());

				return std::chrono::duration_cast<TDuration>(this->_end.value() - this->_start.value());
			}

			static Stopwatch StartNew() noexcept;
		};
	}
}
