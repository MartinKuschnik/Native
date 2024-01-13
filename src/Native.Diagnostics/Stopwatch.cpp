#include "Stopwatch.h"

namespace Native
{
	namespace Diagnostics
	{
		namespace chrono = std::chrono;

		Stopwatch::Stopwatch(const bool start) noexcept
		{
			if (start)
				this->start();
		}

		void Stopwatch::start() noexcept
		{
			this->_start = std::optional<std::chrono::time_point<std::chrono::steady_clock>>(std::chrono::high_resolution_clock::now());
		}

		void Stopwatch::stop() noexcept
		{
			if (this->_start.has_value())
				this->_end = std::optional<std::chrono::time_point<std::chrono::steady_clock>>(std::chrono::high_resolution_clock::now());
		}


		Stopwatch Stopwatch::StartNew() noexcept
		{
			return Stopwatch(true);
		}
	}
}