#pragma once

#include <optional>
#include <thread>

#include "Handle.h"
#include "SecurityIdentifier.h"

namespace Native
{
	namespace Windows
	{
		/// <summary>
		/// Represents a Windows user account.
		/// </summary>
		class WindowsIdentity
		{
		public:

			/// <summary>
			/// Constructs a WindowsIdentity by taking ownership of a token handle.
			/// </summary>
			/// <param name="token_handle">A Handle that represents a security token. The handle is expected to be valid for representing an identity token.</param>
			WindowsIdentity(Handle&& token_handle) noexcept;

			virtual ~WindowsIdentity() noexcept = default;

			/// <summary>
			/// Retrieves the SecurityIdentifier (SID) for the user associated with this WindowsIdentity.
			/// </summary>
			/// <returns>The SecurityIdentifier (SID) for the user associated with this WindowsIdentity.</returns>
			[[nodiscard]]
			SecurityIdentifier get_user() const;

			/// <summary>
			/// Retrieves the WindowsIdentity that represents the current Windows user for the executing context.
			/// </summary>
			/// <returns>A WindowsIdentity object representing the current Windows account (for the executing thread or process).
			/// </returns>
			/// <remarks>
			/// The method first attempts to obtain the WindowsIdentity for the current thread. If the thread is  not impersonating a user,
			/// it falls back to the WindowsIdentity of the current process.
			/// </remarks>
			[[nodiscard]]
			static WindowsIdentity GetCurrent();

		private:

			Handle _tokenHandle;

		};
	}
}