#pragma once

#include "CancellationToken.h"

namespace Native
{
	namespace ServiceModel
	{
		class ServiceBase
		{
		public:

			virtual ~ServiceBase() noexcept = default;

			virtual bool Initialize() noexcept = 0;

			virtual void Run(Threading::CancellationToken cancellationToken) noexcept = 0;
		};
	}
}

