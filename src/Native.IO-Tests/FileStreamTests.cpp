#include "CppUnitTest.h"

#include <array>
#include <filesystem>

#include "Guid.h"
#include "FileStream.h"
#include "InvalidOperationException.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace Native;
using namespace Native::IO;

namespace NativeIOTests
{
	TEST_CLASS(FileStreamTests)
	{
	public:

		TEST_METHOD(write_and_read_to_temp_file)
		{
			// Arrange
			const std::filesystem::path temp_file_name = std::filesystem::temp_directory_path() / std::format("{0}.txt", Native::Guid::NewGuid().string());

			const std::string input_string = "This is the data that is written and read to/from the File.";
			std::string output_string(512, ' ');

			FileStream file_stream(temp_file_name, FileMode::CreateNew, FileAccess::ReadWrite, FileShare::None);

			// Act
			file_stream.write(input_string.data(), input_string.size());

			file_stream.flush();

			const uint64_t pos_after_seek = file_stream.seek(0, SeekOrigin::Begin);

			const size_t read = file_stream.read(output_string.data(), output_string.size());

			file_stream.close();

			// Assert
			Assert::AreEqual(std::string_view(input_string), std::string_view(output_string.data(), read));
		}

		TEST_METHOD(write_and_shrink_temp_file)
		{
			// Arrange
			constexpr uint64_t shrinked_lengt = 10;

			const std::filesystem::path temp_file_name = std::filesystem::temp_directory_path() / std::format("{0}.txt", Native::Guid::NewGuid().string());

			const std::string input_string = "This is the data that is written and read to/from the File.";

			FileStream file_stream(temp_file_name, FileMode::CreateNew, FileAccess::ReadWrite, FileShare::None);

			// Act
			file_stream.write(input_string.data(), input_string.size());

			file_stream.flush();

			file_stream.set_length(shrinked_lengt);

			const uint64_t pos_after_shrink = file_stream.position();

			file_stream.close();

			const uintmax_t size = std::filesystem::file_size(temp_file_name);

			// Assert
			Assert::AreEqual(shrinked_lengt, size);
			Assert::AreEqual(shrinked_lengt, pos_after_shrink);
		}
	};
}