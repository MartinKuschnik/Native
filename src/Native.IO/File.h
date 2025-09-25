#pragma once

#include <filesystem>

#include <FileAttributes.h>

namespace Native
{
	namespace IO
	{
		class File
		{
		public:

			// static class
			File() = delete;

			/// <summary>
			/// Gets the FileAttributes of the file on the path.
			/// </summary>
			/// <param name="path">The path to the file.</param>
			static FileAttributes GetAttributes(const std::filesystem::path path);

			/// <summary>
			/// Sets the specified FileAttributes of the file on the specified path.
			/// </summary>
			/// <param name="path">The path to the file.</param>
			/// <param name="attributes">A bitwise combination of the enumeration values.</param>
			static void SetAttributes(const std::filesystem::path path, const FileAttributes attributes);
		};
	}
}