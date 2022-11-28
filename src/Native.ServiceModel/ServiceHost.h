#pragma once

#include <type_traits>

#include "ServiceBase.h"
#include "CancellationToken.h"

namespace Native
{
	namespace ServiceModel
	{
		template<class T>
			requires std::is_base_of<ServiceBase, T>::value
		&& std::is_default_constructible<T>::value
			class ServiceHost
		{
		public:
			ServiceHost() = delete; //static class

			static int Run() noexcept
			{
				T service;
				Threading::CancellationToken cancellationToken;
								
				if (service.Initialize())
				{
					service.Run(cancellationToken);
				}

				return 0;
			}
		};
	}
}

