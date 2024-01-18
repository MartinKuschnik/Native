#pragma once

#include <filesystem>

#include "Handle.h"

#include "Stream.h"
#include "FileAccess.h"
#include "FileMode.h"
#include "FileShare.h"

namespace Native
{
	namespace IO
	{
		class FileStream : public Stream
		{
		public:
			FileStream(const std::filesystem::path path, const FileMode mode, const FileAccess access, const FileShare share);

			~FileStream() noexcept;

		private:

			const Native::Windows::Handle _handle;

			static Native::Windows::Handle OpenFile(const std::filesystem::path path, const FileMode mode, const FileAccess access, const FileShare share);
		};
	}
}