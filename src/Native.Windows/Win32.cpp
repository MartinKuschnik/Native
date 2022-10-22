#include "Win32.h"

namespace Native
{
	namespace Windows
	{
		std::string Win32::GetWin32ErrorMessage(DWORD errorCode)
		{
			LPSTR messageBuffer = nullptr;
			const size_t size = FormatMessageA(
				FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_MAX_WIDTH_MASK,
				nullptr,
				errorCode,
				MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
				(LPSTR)&messageBuffer,
				0,
				nullptr);

			std::string message(messageBuffer, size == 0 ? 0 : size - 1); // because the trailing white space

			//Free the buffer.
			LocalFree(messageBuffer);

			return message;
		}
	}
}