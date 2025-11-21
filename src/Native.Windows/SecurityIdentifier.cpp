#include "SecurityIdentifier.h"

#include <memory>

namespace Native
{
	namespace Windows
	{
		SecurityIdentifier SecurityIdentifier::FromSid(const PSID sid)
		{
			SecurityIdentifier security_dentifier = *reinterpret_cast<const SecurityIdentifier*>(sid);

			// clear unused sub-authority slots to ensure consistent SID representation
			ZeroMemory(&security_dentifier.SubAuthority[security_dentifier.SubAuthorityCount], (SID_MAX_SUB_AUTHORITIES - security_dentifier.SubAuthorityCount) * sizeof(DWORD));

			return security_dentifier;
		}

		SecurityIdentifier::operator const std::string() const
		{
			LPSTR sid_as_string = nullptr;

			if (!ConvertSidToStringSidA(const_cast<void*>(reinterpret_cast<const void*>(this)), &sid_as_string))
				throw Win32Exception(GetLastError(), nameof(ConvertSidToStringSidA));

			std::string result = sid_as_string;

			LocalFree(sid_as_string);

			return result;
		}

		SecurityIdentifier::operator const std::wstring() const
		{
			LPWSTR sid_as_string = nullptr;

			if (!ConvertSidToStringSidW(const_cast<void*>(reinterpret_cast<const void*>(this)), &sid_as_string))
				throw Win32Exception(GetLastError(), nameof(ConvertSidToStringSidW));

			std::wstring result = sid_as_string;

			LocalFree(sid_as_string);

			return result;
		}
	}
}
