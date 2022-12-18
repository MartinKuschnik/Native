#pragma once

#include <string>

#include "CancellationToken.h"

#include "RunMode.h"

namespace Native
{
	namespace ServiceModel
	{
		class ServiceBase
		{
		public:

			virtual ~ServiceBase() noexcept = default;

			virtual bool initialize(RunMode runMode) noexcept = 0;

			virtual void run(Threading::CancellationToken&& cancellationToken) noexcept = 0;

			virtual std::string name() const = 0;
		};
	}
}

