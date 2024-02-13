#pragma once

#include <cstdint>

#include <optional>

#include "CancellationToken.h"

namespace Native
{
	namespace Threading
	{
		namespace Tasks
		{
			struct ParallelOptions
			{
				CancellationToken CancellationToken;

				std::optional<uint32_t> MaxDegreeOfParallelism;
			};
		}
	}
}