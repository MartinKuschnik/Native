#include "FileStream.h"

#include <Windows.h>

#include "Win32Exception.h"

namespace Native
{
	namespace IO
	{
		FileStream::FileStream(const std::filesystem::path path, const FileMode mode, const FileAccess access, const FileShare share)
			: _handle(OpenFile(path, mode, access, share))
		{
		}

		FileStream::~FileStream() noexcept
		{
		}

		Native::Windows::Handle FileStream::OpenFile(const std::filesystem::path path, const FileMode mode, const FileAccess access, const FileShare share)
		{
			const std::wstring path_as_wstring = path.wstring();

			HANDLE handle = CreateFileW(path_as_wstring.c_str(), static_cast<DWORD>(access), static_cast<DWORD>(share), nullptr, static_cast<DWORD>(mode), FILE_ATTRIBUTE_NORMAL, nullptr);

			if (handle == INVALID_HANDLE_VALUE)
				throw Native::Windows::Win32Exception(GetLastError(), nameof(CreateFileW));

			return Native::Windows::Handle(handle);
		}
	}
}