#pragma once

#include <string>

#include <Windows.h>

#include "SeriveType.h"
#include "SeriveStatus.h"
#include "SeriveControlsAccepted.h"

#include "Win32Exception.h"

namespace Native
{
	namespace ServiceModel
	{
		class ServiceStatusHandle
		{
		private:

			const SERVICE_STATUS_HANDLE _handle = NULL;

			const SeriveType _serviceType;

			constexpr ServiceStatusHandle(const SERVICE_STATUS_HANDLE handle, const SeriveType serviceType) noexcept
				: _handle(handle),
				_serviceType(serviceType)
			{
			}

		public:

			ServiceStatusHandle() noexcept = delete;

			constexpr ServiceStatusHandle(const ServiceStatusHandle&) = delete;

			constexpr ServiceStatusHandle(ServiceStatusHandle&& other) noexcept
				: _handle(other._handle),
				_serviceType(other._serviceType)
			{
			}

			template<typename T>
			static ServiceStatusHandle Open(std::string_view service_name, const SeriveType serviceType, LPHANDLER_FUNCTION_EX handler_poc, T* context)
			{
				const SERVICE_STATUS_HANDLE handle = RegisterServiceCtrlHandlerExA(service_name.data(), handler_poc, context);

				return ServiceStatusHandle(handle, serviceType);
			}

			constexpr bool is_valid() const noexcept
			{
				return this->_handle != NULL;
			}

			void set_status(const SeriveStatus status) const
			{
				SERVICE_STATUS service_status{ .dwServiceType = static_cast<DWORD>(this->_serviceType), .dwCurrentState = static_cast<DWORD>(status) };

				bool success = SetServiceStatus(this->_handle, &service_status);

				if (!success)
					throw Windows::Win32Exception(GetLastError(), nameof(SetServiceStatus));
			}

			void set_status(const SeriveStatus status, const SeriveControlsAccepted controls_accepted) const
			{
				SERVICE_STATUS service_status{ .dwServiceType = static_cast<DWORD>(this->_serviceType), .dwCurrentState = static_cast<DWORD>(status), .dwControlsAccepted = static_cast<DWORD>(controls_accepted) };

				bool success = SetServiceStatus(this->_handle, &service_status);

				if (!success)
					throw Windows::Win32Exception(GetLastError(), nameof(SetServiceStatus));
			}
		};
	}
}
