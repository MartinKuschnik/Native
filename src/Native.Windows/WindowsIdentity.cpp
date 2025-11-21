#include "WindowsIdentity.h"

#include <memory>

#include <Windows.h>
#include <sddl.h>

#include "Handle.h"
#include "Win32Exception.h"
#include "NotSupportedException.h"

namespace Native
{
	namespace Windows
	{

		WindowsIdentity::WindowsIdentity(Handle&& token_handle) noexcept
			: _tokenHandle(std::move(token_handle))
		{
		}

		SecurityIdentifier WindowsIdentity::get_user() const
		{
			std::byte buffer[TOKEN_USER_MAX_SIZE];

			DWORD token_info_size = 0;

			if (!GetTokenInformation(this->_tokenHandle, TokenUser, &buffer, sizeof(buffer), &token_info_size))
				throw Windows::Win32Exception(GetLastError(), nameof(GetTokenInformation));

			const TOKEN_USER* user_token = reinterpret_cast<TOKEN_USER*>(&buffer);

			return SecurityIdentifier::FromSid(user_token->User.Sid);
		}

		WindowsIdentity WindowsIdentity::GetCurrent()
		{
			HANDLE token_handle;

			// Attempt to open the current thread's access token first.
			// Thread tokens have higher priority and represent the most specific security context.
			// If a thread token exists, it overrides the process token for the current execution context.
			if (!OpenThreadToken(GetCurrentThread(), TOKEN_QUERY, TRUE, &token_handle))
			{
				const DWORD last_error = GetLastError();

				// ERROR_NO_TOKEN is expected when no thread token is assigned.
				// Any other error indicates a real failure and should be propagated.
				if (last_error != ERROR_NO_TOKEN)
					throw Windows::Win32Exception(GetLastError(), nameof(OpenThreadToken));

				// Fall back to the process token if thread token is not available.
				// The process token represents the security context of the entire process.
				// This is the default identity when no thread-specific token is set.
				if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &token_handle))
					throw Windows::Win32Exception(GetLastError(), nameof(OpenProcessToken));

				// Return the process token.
				return WindowsIdentity(token_handle);
			}

			// Return the thread token if successfully opened.
			return WindowsIdentity(token_handle);
		}
	}
}
