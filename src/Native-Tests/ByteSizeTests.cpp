#include "CppUnitTest.h"

#include "ByteSize.h"

using namespace std::string_literals;
using namespace Native;
using namespace Native::ByteSizeLiterals;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace NativeTests
{
	TEST_CLASS(ByteSizeTests)
	{

	public:
		TEST_METHOD(Format_1024_B_to_1_KB)
		{
			// Arrange
			const ByteSize byte_size = 1024_B;

			// Act
			const std::string as_string = std::format("{0}", byte_size);

			// Assert
			Assert::AreEqual("1.00 KB"s, as_string);
		}

		TEST_METHOD(Format_1024_KB_to_1_MB)
		{
			// Arrange
			const ByteSize byte_size = 1024_KB;

			// Act
			const std::string as_string = std::format("{0}", byte_size);

			// Assert
			Assert::AreEqual("1.00 MB"s, as_string);
		}

		TEST_METHOD(Format_1024_MB_to_1_GB)
		{
			// Arrange
			const ByteSize byte_size = 1024_MB;

			// Act
			const std::string as_string = std::format("{0}", byte_size);

			// Assert
			Assert::AreEqual("1.00 GB"s, as_string);
		}

		TEST_METHOD(Format_1024_GB_to_1_TB)
		{
			// Arrange
			const ByteSize byte_size = 1024_GB;

			// Act
			const std::string as_string = std::format("{0}", byte_size);

			// Assert
			Assert::AreEqual("1.00 TB"s, as_string);
		}

		TEST_METHOD(Format_1024_TB_to_1_PB)
		{
			// Arrange
			const ByteSize byte_size = 1024_TB;

			// Act
			const std::string as_string = std::format("{0}", byte_size);

			// Assert
			Assert::AreEqual("1.00 PB"s, as_string);
		}
	};
}
