#pragma once

#include <format>
#include <array>

#include <Windows.h>
#include <sddl.h>

#include "Win32Exception.h"

namespace Native
{
	namespace Windows
	{
		/// <summary>
		/// Same like SID structure but with space for max sub authority.
		/// This allows to handle SIDs without heap allocations.
		/// </summary>
		class SecurityIdentifier
		{
		public:

			// Same like SID structure but with space for max sub authority.
			BYTE  Revision;
			BYTE  SubAuthorityCount;
			SID_IDENTIFIER_AUTHORITY IdentifierAuthority;
			DWORD SubAuthority[SID_MAX_SUB_AUTHORITIES];

			/// <summary>
			/// Creates SecurityIdentifier from PSID.
			/// </summary>
			/// <param name="sid"></param>
			/// <returns>The created SecurityIdentifier</returns>
			static SecurityIdentifier FromSid(const PSID sid);

			operator const std::string() const;

			operator const std::wstring() const;
		};

		static_assert(sizeof(SecurityIdentifier) == SECURITY_MAX_SID_SIZE, "SecurityIdentifier has wrong size!");
		static_assert(offsetof(SecurityIdentifier, Revision) == offsetof(SID, Revision), "Revision has wrong Offset!");
		static_assert(offsetof(SecurityIdentifier, SubAuthorityCount) == offsetof(SID, SubAuthorityCount), "SubAuthorityCount has wrong Offset!");
		static_assert(offsetof(SecurityIdentifier, SubAuthority) == offsetof(SID, SubAuthority), "SubAuthorityCount has wrong Offset!");
	}
}

template <>
struct std::formatter<Native::Windows::SecurityIdentifier> : std::formatter<string_view> {
	auto format(const Native::Windows::SecurityIdentifier& sid, std::format_context& ctx) const
	{
		const std::string stringSid = sid;

		return std::formatter<string_view>::format(stringSid, ctx);
	}
};

template <>
struct std::formatter<Native::Windows::SecurityIdentifier, wchar_t> : std::formatter<wstring_view, wchar_t> {
	auto format(const Native::Windows::SecurityIdentifier& sid, std::wformat_context& ctx) const
	{
		const std::wstring stringSid = sid;

		return std::formatter<wstring_view, wchar_t>::format(stringSid, ctx);
	}
};