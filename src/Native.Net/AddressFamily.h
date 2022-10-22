#pragma once

namespace Native
{
	namespace Net
	{
		/// <summary>
		/// Specifies the addressing scheme that an instance of the Socket class can use.
		/// </summary>
		enum class AddressFamily : unsigned char
		{
			/// <summary>
			/// Address for IP version 4.
			/// </summary>
			InterNetwork,

			/// <summary>
			/// Address for IP version 6.
			/// </summary>
			InterNetworkV6
		};
	}
}