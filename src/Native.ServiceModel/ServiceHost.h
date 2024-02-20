#pragma once

#include <type_traits>

#include <conio.h>
#include <iostream>

#include "CancellationTokenSource.h"
#include "Console.h"
#include "ServiceBase.h"
#include "ServiceStatusHandle.h"

namespace Native
{
	namespace ServiceModel
	{
		template<class T>
			requires std::is_base_of<ServiceBase, T>::value
		          && std::is_default_constructible<T>::value
			class ServiceHost
		{
		private:

			struct ServiceContext
			{
				ServiceStatusHandle* StatusHandle;
				Threading::CancellationTokenSource* CancellationTokenSource;
			};

			static inline T instance;

		public:
			ServiceHost() = delete; //static class

			static int Run() noexcept
			{
				const DWORD last_error = RunAsService();

				if (last_error == ERROR_FAILED_SERVICE_CONTROLLER_CONNECT) // This error is returned if the program is being run as a console application rather than as a service.
					return RunAsConsole();

				return last_error;
			}

		private:

			static int RunAsConsole()
			{
				Threading::CancellationTokenSource cts;

				auto subbrciption = Native::Console::CancelKeyPress.subscribe(
					[&cts](auto& args)
					{
						args.cancel();
						cts.cancel();
					});

				if (IsDebuggerPresent())
				{
					std::string serviceName = instance.name();
					SetConsoleTitleA(serviceName.data());
				}

				if (instance.initialize(RunMode::Console))
				{
					instance.run(cts.Token);
				}

				std::cout << "Press any key to continue . . ." << std::endl;
				_getch(); // wait for key press

				return 0;
			}

			static DWORD RunAsService()
			{
				SERVICE_TABLE_ENTRYA service_table[] =
				{
					{ instance.name().data(), ServiceHost<T>::ServiceMain },
					{ nullptr, nullptr } // MSDN:  The members of the last entry in the table must have NULL values to designate the end of the table.
				};

				const bool success = StartServiceCtrlDispatcherA(service_table);

				if (!success)
					return GetLastError();

				return NO_ERROR;
			}

			static void __stdcall ServiceMain(DWORD dwNumServicesArgs, char** lpServiceArgVectors)
			{
				Threading::CancellationTokenSource cancellation_token_source;

				ServiceContext context { .CancellationTokenSource = &cancellation_token_source };

				ServiceStatusHandle status_handle = ServiceStatusHandle::Open(instance.name(), SeriveType::Win32_Own_Process, ServiceHost<T>::ServiceCtrHandler, &context);

				context.StatusHandle = &status_handle;

				status_handle.set_status(SeriveStatus::Start_Pending);

				if (instance.initialize(RunMode::Service))
				{
					status_handle.set_status(SeriveStatus::Running, SeriveControlsAccepted::Accept_Stop | SeriveControlsAccepted::Accept_Shutdown);

					instance.run(cancellation_token_source.Token);
				}

				status_handle.set_status(SeriveStatus::Stopped);
			}

			static DWORD __stdcall ServiceCtrHandler(DWORD dwControl, DWORD dwEventType, void* lpEventData, void* lpContext)
			{
				const ServiceContext* context = reinterpret_cast<ServiceContext*>(lpContext);

				switch (dwControl)
				{
				case SERVICE_CONTROL_SHUTDOWN:
				case SERVICE_CONTROL_STOP:
				{
					context->StatusHandle->set_status(SeriveStatus::Stop_Pending);

					context->CancellationTokenSource->cancel();

					return NO_ERROR;
				}
				default:
					return ERROR_CALL_NOT_IMPLEMENTED;
				}
			}

		};
	}
}

