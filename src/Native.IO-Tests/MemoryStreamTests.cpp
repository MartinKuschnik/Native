#include "CppUnitTest.h"

#include <array>

#include "MemoryStream.h"
#include "InvalidOperationException.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace Native;
using namespace Native::IO;

namespace NativeIOTests
{
	TEST_CLASS(MemoryStreamTests)
	{
	public:

		TEST_METHOD(seek_0_to_end_is_size)
		{
			// Arrange
			constexpr uint64_t stream_size = 512;

			MemoryStream mem_stream(stream_size);

			// Act
			const uint64_t seek_result = mem_stream.seek(0, SeekOrigin::End);
			const uint64_t pos_after_seek = mem_stream.position();

			// Assert
			Assert::AreEqual(stream_size, seek_result);
			Assert::AreEqual(stream_size, pos_after_seek);
		}

		TEST_METHOD(seek_size_to_begin_is_size)
		{
			// Arrange
			constexpr uint64_t stream_size = 512;

			MemoryStream mem_stream(stream_size);

			// Act
			const uint64_t seek_result = mem_stream.seek(stream_size, SeekOrigin::Begin);
			const uint64_t pos_after_seek = mem_stream.position();

			// Assert
			Assert::AreEqual(stream_size, seek_result);
			Assert::AreEqual(stream_size, pos_after_seek);
		}

		TEST_METHOD(seek_half_size_to_current_twice_is_size)
		{
			// Arrange
			constexpr uint64_t stream_size = 512;
			constexpr uint64_t half_stream_size = stream_size / 2;

			MemoryStream mem_stream(stream_size);

			// Act
			const uint64_t first_seek_result = mem_stream.seek(half_stream_size, SeekOrigin::Current);
			const uint64_t pos_after_first_seek = mem_stream.position();

			const uint64_t second_seek_result = mem_stream.seek(half_stream_size, SeekOrigin::Current);
			const uint64_t pos_after_second_seek = mem_stream.position();

			// Assert
			Assert::AreEqual(half_stream_size, first_seek_result);
			Assert::AreEqual(half_stream_size, pos_after_first_seek);
			Assert::AreEqual(stream_size, second_seek_result);
			Assert::AreEqual(stream_size, pos_after_second_seek);
		}

		TEST_METHOD(seek_bigger_than_size_to_begin_throws_InvalidOperationException)
		{
			// Arrange
			constexpr uint64_t stream_size = 512;

			MemoryStream mem_stream(stream_size);

			// Act
			Assert::ExpectException<InvalidOperationException>([&mem_stream]() {
				mem_stream.seek(stream_size + 1, SeekOrigin::Begin);
				});
		}

		TEST_METHOD(seek_bigger_than_size_to_end_throws_InvalidOperationException)
		{
			// Arrange
			constexpr uint64_t stream_size = 512;

			MemoryStream mem_stream(stream_size);

			// Act
			Assert::ExpectException<InvalidOperationException>([&mem_stream]() {
				mem_stream.seek(stream_size + 1, SeekOrigin::End);
				});
		}

		TEST_METHOD(seek_bigger_than_size_to_current_throws_InvalidOperationException)
		{
			// Arrange
			constexpr uint64_t stream_size = 512;

			MemoryStream mem_stream(stream_size);

			// Act
			Assert::ExpectException<InvalidOperationException>([&mem_stream]() {  
					mem_stream.seek(stream_size + 1, SeekOrigin::Current);
				});
		}

		TEST_METHOD(can_read_is_true_for_readonly_stream)
		{
			// Arrange
			MemoryStream mem_stream(512, false);

			// Act
			const bool can_read = mem_stream.can_read();

			// Assert
			Assert::IsTrue(can_read);
		}

		TEST_METHOD(can_read_is_true_for_writable_stream)
		{
			// Arrange
			MemoryStream mem_stream(512, true);

			// Act
			const bool can_read = mem_stream.can_read();

			// Assert
			Assert::IsTrue(can_read);
		}

		TEST_METHOD(can_write_is_false_for_readonly_stream)
		{
			// Arrange
			MemoryStream mem_stream(512, false);

			// Act
			const bool can_write = mem_stream.can_write();

			// Assert
			Assert::IsFalse(can_write);
		}

		TEST_METHOD(can_write_is_true_for_writable_stream)
		{
			// Arrange
			MemoryStream mem_stream(512, true);

			// Act
			const bool can_write = mem_stream.can_write();

			// Assert
			Assert::IsTrue(can_write);
		}

		TEST_METHOD(write_with_buffer_bigger_than_size_throws_InvalidOperationException)
		{
			// Arrange
			constexpr uint64_t stream_size = 512;
			std::byte buffer[stream_size + 1];

			MemoryStream mem_stream(stream_size);

			// Act
			Assert::ExpectException<InvalidOperationException>([&mem_stream, buffer]() { 
				mem_stream.write(buffer, sizeof(buffer));				
			});
		}
		TEST_METHOD(write_moves_position)
		{
			// Arrange
			constexpr size_t buffer_size = 256;

			std::array<std::byte, buffer_size> buffer;

			MemoryStream mem_stream(512);

			// Act
			mem_stream.write(buffer.data(), buffer.size());

			// Assert
			Assert::AreEqual(buffer_size, static_cast<size_t>(mem_stream.position()));
		}

		TEST_METHOD(read_written_data)
		{
			// Arrange
			const std::string input_string = "This is the data that is written and read to/from the MemoryStream.";

			std::string output_string(512, ' ');

			MemoryStream mem_stream(512);

			// Act
			mem_stream.write(input_string.data(), input_string.size());
			mem_stream.seek(0, SeekOrigin::Begin);
			const size_t read = mem_stream.read(output_string.data(), input_string.size());

			// Assert
			Assert::AreEqual(std::string_view(input_string), std::string_view(output_string.data(), read));
		}

		TEST_METHOD(read_does_not_exeed_buffer)
		{
			// Arrange
			constexpr size_t stream_size = 512;

			MemoryStream mem_stream(stream_size);
			std::array<char, stream_size * 2> buffer;

			// Act
			const size_t read = mem_stream.read(buffer.data(), buffer.size());

			// Assert
			Assert::AreEqual(stream_size, read);
		}

	};
}
