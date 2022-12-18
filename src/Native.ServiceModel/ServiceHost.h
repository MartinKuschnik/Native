#pragma once

#include <type_traits>

#include <iostream>
#include <conio.h>

#include "CancellationTokenSource.h"
#include "ServiceBase.h"
#include "Console.h"

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
				return RunAsConsole();
			}

			static int RunAsConsole()
			{
				T service;
				Threading::CancellationTokenSource cts;

				auto subbrciption = Native::Console::CancelKeyPress.subscribe(
					[&cts](auto& args)
					{
						args.cancel();
						cts.cancel();
					});

				if (IsDebuggerPresent())
				{
					std::string serviceName = service.name();
					SetConsoleTitleA(serviceName.data());
				}

				if (service.initialize(RunMode::Console))
				{
					service.run(cts.Token);
				}

				std::cout << "Press any key to continue . . ." << std::endl;
				_getch(); // wait for key press

				return 0;
			}

			static int RunAsService()
			{
				// ToDo: implement ServiceHost<T>::RunAsService
				return ERROR_CALL_NOT_IMPLEMENTED;
			}
		};
	}
}

