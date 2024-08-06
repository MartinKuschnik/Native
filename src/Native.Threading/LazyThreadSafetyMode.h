#pragma once

namespace Native
{
	namespace Threading
	{
		enum class LazyThreadSafetyMode
		{
			None = 0,
			PublicationOnly = 1,
			ExecutionAndPublication = 2
		};
	}
}