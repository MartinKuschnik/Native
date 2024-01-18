#pragma once

namespace Native
{
	namespace IO
	{
		/// <summary>
		/// Specifies the position in a stream to use for seeking.
		/// </summary>
		enum class SeekOrigin
		{
			/// <summary>
			/// Specifies the beginning of a stream.
			/// </summary>
			Begin = 0,
			
			/// <summary>
			/// Specifies the current position within a stream.
			/// </summary>
			Current = 1,
			
			/// <summary>
			/// Specifies the end of a stream.
			/// </summary>
			End  = 2			
		};
	}
}