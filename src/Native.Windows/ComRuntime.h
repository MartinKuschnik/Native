#pragma once

#include <ComObject.h>
#include <HResultException.h>

namespace Native
{
	namespace Windows
	{
		namespace COM
		{
			class ComRuntime
			{
			public:

				ComRuntime(const COINIT flags);
				ComRuntime(const int flags);

				ComRuntime() = delete;
				ComRuntime(const ComRuntime&) = delete;
				ComRuntime(ComRuntime&&) = delete;

				~ComRuntime() noexcept;

				void initialize_security(DWORD dwAuthnLevel, DWORD dwImpLevel, DWORD dwCapabilities);

				void initialize_security(PSECURITY_DESCRIPTOR pSecDesc, LONG cAuthSvc, SOLE_AUTHENTICATION_SERVICE* asAuthSvc, void* pReserved1, DWORD dwAuthnLevel, DWORD dwImpLevel, void* pAuthList, DWORD dwCapabilities, void* pReserved3);

				template<typename T>
					requires std::is_base_of<IUnknown, T>::value
				ComObject<T> create_instance(REFCLSID rclsid, DWORD dwClsContext, REFIID riid)
				{
					ComObject<T> com_object;

					const HRESULT hres = ::CoCreateInstance(rclsid, nullptr, dwClsContext, riid, com_object);

					if (FAILED(hres))
						throw HResultException(hres, nameof(CoCreateInstance));

					return com_object;
				}

			};
		}
	}
}

