#include "CppUnitTest.h"

#include <array>

#include "Stream.h"
#include "MemoryStream.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace Native;
using namespace Native::IO;

namespace NativeIOTests
{
	TEST_CLASS(StreamTests)
	{
	public:

		TEST_METHOD(copy_to_copies_all_data_with_one_third_of_buffer_size)
		{
			// Arrange
			const std::string input_string = "This is the data that is written and read to/from the MemoryStream.";

			MemoryStream mem_stream_1(input_string.data(), input_string.size());
			MemoryStream mem_stream_2(input_string.size());

			// Act
			const size_t copied = mem_stream_1.copy_to(&mem_stream_2, input_string.length() / 3);

			// Assert
			Assert::AreEqual(std::string_view(input_string), std::string_view(reinterpret_cast<const char*>(mem_stream_2.data()), copied));
		}

		TEST_METHOD(copy_to_copies_all_data_with_full_buffer_size)
		{
			// Arrange
			const std::string input_string = "This is the data that is written and read to/from the MemoryStream.";

			MemoryStream mem_stream_1(input_string.data(), input_string.size());
			MemoryStream mem_stream_2(input_string.size());

			// Act
			const size_t copied = mem_stream_1.copy_to(&mem_stream_2, input_string.length());

			// Assert
			Assert::AreEqual(std::string_view(input_string), std::string_view(reinterpret_cast<const char*>(mem_stream_2.data()), copied));
		}

	};
}