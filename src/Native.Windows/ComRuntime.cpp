#include "ComRuntime.h"

namespace Native
{
	namespace Windows
	{
		namespace COM
		{
			ComRuntime::ComRuntime(const COINIT flags)
				: ComRuntime(static_cast<int>(flags))
			{
			}

			ComRuntime::ComRuntime(const int flags)
			{
				const HRESULT hres = CoInitializeEx(0, flags);

				if (FAILED(hres))
					throw HResultException(hres, nameof(CoInitializeEx));
			}

			ComRuntime::~ComRuntime()
			{
				CoUninitialize();
			}

			void ComRuntime::initialize_security(DWORD dwAuthnLevel, DWORD dwImpLevel, DWORD dwCapabilities)
			{
				this->initialize_security(nullptr, -1, nullptr, nullptr, dwAuthnLevel, dwImpLevel, nullptr, dwCapabilities, nullptr);
			}

			void ComRuntime::initialize_security(PSECURITY_DESCRIPTOR pSecDesc, LONG cAuthSvc, SOLE_AUTHENTICATION_SERVICE* asAuthSvc, void* pReserved1, DWORD dwAuthnLevel, DWORD dwImpLevel, void* pAuthList, DWORD dwCapabilities, void* pReserved3)
			{
				const HRESULT hres = CoInitializeSecurity(pSecDesc, cAuthSvc, asAuthSvc, pReserved1, dwAuthnLevel, dwImpLevel, pAuthList, dwCapabilities, pReserved3);

				if (FAILED(hres))
					throw HResultException(hres, nameof(CoInitializeSecurity));
			}
		}
	}
}