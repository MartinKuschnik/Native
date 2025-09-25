#include "File.h"

#include <Windows.h>

#include <Win32Exception.h>

namespace Native
{
	namespace IO
	{
		FileAttributes File::GetAttributes(const std::filesystem::path path)
		{
			const DWORD attr = ::GetFileAttributesW(path.c_str());

			if (attr == INVALID_FILE_ATTRIBUTES)
			{
				const DWORD error = ::GetLastError();

				throw Native::Windows::Win32Exception(error, nameof(GetFileAttributesW));
			}

			return static_cast<FileAttributes>(attr);
		}

		void File::SetAttributes(const std::filesystem::path path, const FileAttributes attributes)
		{
			BOOL success = ::SetFileAttributesW(path.c_str(), static_cast<DWORD>(attributes));

			if (!success)
			{
				const DWORD error = ::GetLastError();

				throw Native::Windows::Win32Exception(error, nameof(GetFileAttributesW));
			}
		}
	}
}