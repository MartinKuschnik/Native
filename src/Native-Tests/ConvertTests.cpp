#include "CppUnitTest.h"

#include "Convert.h"

using namespace Native;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace NativeTests
{
	TEST_CLASS(ConvertTests)
	{
	public:

		// ToHex Tests for char
		TEST_METHOD(ToHex_Char_Zero_ReturnsZeroZero)
		{
			constexpr std::byte input{ 0x00 };
			const auto result = Convert::ToHex<char>(input);

			Assert::AreEqual('0', result.Char1);
			Assert::AreEqual('0', result.Char2);
		}

		TEST_METHOD(ToHex_Char_MaxValue_ReturnsFF)
		{
			constexpr std::byte input{ 0xFF };
			const auto result = Convert::ToHex<char>(input);

			Assert::AreEqual('F', result.Char1);
			Assert::AreEqual('F', result.Char2);
		}

		TEST_METHOD(ToHex_Char_LowerNibbleOnly_ReturnsZeroF)
		{
			constexpr std::byte input{ 0x0F };
			const auto result = Convert::ToHex<char>(input);

			Assert::AreEqual('0', result.Char1);
			Assert::AreEqual('F', result.Char2);
		}

		TEST_METHOD(ToHex_Char_UpperNibbleOnly_ReturnsFZero)
		{
			constexpr std::byte input{ 0xF0 };
			const auto result = Convert::ToHex<char>(input);

			Assert::AreEqual('F', result.Char1);
			Assert::AreEqual('0', result.Char2);
		}

		TEST_METHOD(ToHex_Char_MixedValue_ReturnsCorrectHex)
		{
			constexpr std::byte input{ 0xA5 };
			const auto result = Convert::ToHex<char>(input);

			Assert::AreEqual('A', result.Char1);
			Assert::AreEqual('5', result.Char2);
		}

		TEST_METHOD(ToHex_Char_AllDigits_ReturnsCorrectHex)
		{
			constexpr std::byte input{ 0x99 };
			const auto result = Convert::ToHex<char>(input);

			Assert::AreEqual('9', result.Char1);
			Assert::AreEqual('9', result.Char2);
		}

		TEST_METHOD(ToHex_Char_Value42_Returns2A)
		{
			constexpr std::byte input{ 42 };
			const auto result = Convert::ToHex<char>(input);

			Assert::AreEqual('2', result.Char1);
			Assert::AreEqual('A', result.Char2);
		}

		// ToHex Tests for wchar_t
		TEST_METHOD(ToHex_WChar_Zero_ReturnsZeroZero)
		{
			constexpr std::byte input{ 0x00 };
			const auto result = Convert::ToHex<wchar_t>(input);

			Assert::AreEqual(L'0', result.Char1);
			Assert::AreEqual(L'0', result.Char2);
		}

		TEST_METHOD(ToHex_WChar_MaxValue_ReturnsFF)
		{
			constexpr std::byte input{ 0xFF };
			const auto result = Convert::ToHex<wchar_t>(input);

			Assert::AreEqual(L'F', result.Char1);
			Assert::AreEqual(L'F', result.Char2);
		}

		TEST_METHOD(ToHex_WChar_LowerNibbleOnly_ReturnsZeroF)
		{
			constexpr std::byte input{ 0x0F };
			const auto result = Convert::ToHex<wchar_t>(input);

			Assert::AreEqual(L'0', result.Char1);
			Assert::AreEqual(L'F', result.Char2);
		}

		TEST_METHOD(ToHex_WChar_UpperNibbleOnly_ReturnsFZero)
		{
			constexpr std::byte input{ 0xF0 };
			const auto result = Convert::ToHex<wchar_t>(input);

			Assert::AreEqual(L'F', result.Char1);
			Assert::AreEqual(L'0', result.Char2);
		}

		TEST_METHOD(ToHex_WChar_MixedValue_ReturnsCorrectHex)
		{
			constexpr std::byte input{ 0xA5 };
			const auto result = Convert::ToHex<wchar_t>(input);

			Assert::AreEqual(L'A', result.Char1);
			Assert::AreEqual(L'5', result.Char2);
		}

		TEST_METHOD(ToHex_WChar_AllDigits_ReturnsCorrectHex)
		{
			constexpr std::byte input{ 0x99 };
			const auto result = Convert::ToHex<wchar_t>(input);

			Assert::AreEqual(L'9', result.Char1);
			Assert::AreEqual(L'9', result.Char2);
		}

		// FromHex Tests for char - Success cases
		TEST_METHOD(FromHex_Char_ValidZero_ReturnsTrue)
		{
			constexpr DoubleChar<char> input{ '0', '0' };
			std::byte result;
			const bool success = Convert::FromHex(input, result);

			Assert::IsTrue(success);
			Assert::AreEqual(static_cast<unsigned char>(0x00), static_cast<unsigned char>(result));
		}

		TEST_METHOD(FromHex_Char_ValidFF_ReturnsTrue)
		{
			constexpr DoubleChar<char> input{ 'F', 'F' };
			std::byte result;
			const bool success = Convert::FromHex(input, result);

			Assert::IsTrue(success);
			Assert::AreEqual(static_cast<unsigned char>(0xFF), static_cast<unsigned char>(result));
		}

		TEST_METHOD(FromHex_Char_ValidLowerCase_ReturnsTrue)
		{
			constexpr DoubleChar<char> input{ 'a', 'f' };
			std::byte result;
			const bool success = Convert::FromHex(input, result);

			Assert::IsTrue(success);
			Assert::AreEqual(static_cast<unsigned char>(0xAF), static_cast<unsigned char>(result));
		}

		TEST_METHOD(FromHex_Char_ValidMixedCase_ReturnsTrue)
		{
			constexpr DoubleChar<char> input{ 'A', 'f' };
			std::byte result;
			const bool success = Convert::FromHex(input, result);

			Assert::IsTrue(success);
			Assert::AreEqual(static_cast<unsigned char>(0xAF), static_cast<unsigned char>(result));
		}

		TEST_METHOD(FromHex_Char_ValidDigits_ReturnsTrue)
		{
			constexpr DoubleChar<char> input{ '9', '9' };
			std::byte result;
			const bool success = Convert::FromHex(input, result);

			Assert::IsTrue(success);
			Assert::AreEqual(static_cast<unsigned char>(0x99), static_cast<unsigned char>(result));
		}

		TEST_METHOD(FromHex_Char_ValidMixed_ReturnsTrue)
		{
			constexpr DoubleChar<char> input{ '2', 'A' };
			std::byte result;
			const bool success = Convert::FromHex(input, result);

			Assert::IsTrue(success);
			Assert::AreEqual(static_cast<unsigned char>(42), static_cast<unsigned char>(result));
		}

		// FromHex Tests for char - Error cases
		TEST_METHOD(FromHex_Char_InvalidFirstChar_ReturnsFalse)
		{
			constexpr DoubleChar<char> input{ 'G', '0' };
			std::byte result;
			const bool success = Convert::FromHex(input, result);

			Assert::IsFalse(success);
		}

		TEST_METHOD(FromHex_Char_InvalidSecondChar_ReturnsFalse)
		{
			constexpr DoubleChar<char> input{ '0', 'G' };
			std::byte result;
			const bool success = Convert::FromHex(input, result);

			Assert::IsFalse(success);
		}

		TEST_METHOD(FromHex_Char_InvalidBothChars_ReturnsFalse)
		{
			constexpr DoubleChar<char> input{ 'X', 'Y' };
			std::byte result;
			const bool success = Convert::FromHex(input, result);

			Assert::IsFalse(success);
		}

		TEST_METHOD(FromHex_Char_InvalidSpecialChar_ReturnsFalse)
		{
			constexpr DoubleChar<char> input{ '@', '0' };
			std::byte result;
			const bool success = Convert::FromHex(input, result);

			Assert::IsFalse(success);
		}

		TEST_METHOD(FromHex_Char_InvalidSpace_ReturnsFalse)
		{
			constexpr DoubleChar<char> input{ ' ', '0' };
			std::byte result;
			const bool success = Convert::FromHex(input, result);

			Assert::IsFalse(success);
		}

		TEST_METHOD(FromHex_Char_InvalidNegativeValue_ReturnsFalse)
		{
			constexpr DoubleChar<char> input{ '/', '0' };
			std::byte result;
			const bool success = Convert::FromHex(input, result);

			Assert::IsFalse(success);
		}

		TEST_METHOD(FromHex_Char_InvalidColon_ReturnsFalse)
		{
			constexpr DoubleChar<char> input{ ':', '0' };
			std::byte result;
			const bool success = Convert::FromHex(input, result);

			Assert::IsFalse(success);
		}

		// FromHex Tests for wchar_t - Success cases
		TEST_METHOD(FromHex_WChar_ValidZero_ReturnsTrue)
		{
			constexpr DoubleChar<wchar_t> input{ L'0', L'0' };
			std::byte result;
			const bool success = Convert::FromHex(input, result);

			Assert::IsTrue(success);
			Assert::AreEqual(static_cast<unsigned char>(0x00), static_cast<unsigned char>(result));
		}

		TEST_METHOD(FromHex_WChar_ValidFF_ReturnsTrue)
		{
			constexpr DoubleChar<wchar_t> input{ L'F', L'F' };
			std::byte result;
			const bool success = Convert::FromHex(input, result);

			Assert::IsTrue(success);
			Assert::AreEqual(static_cast<unsigned char>(0xFF), static_cast<unsigned char>(result));
		}

		TEST_METHOD(FromHex_WChar_ValidLowerCase_ReturnsTrue)
		{
			constexpr DoubleChar<wchar_t> input{ L'a', L'f' };
			std::byte result;
			const bool success = Convert::FromHex(input, result);

			Assert::IsTrue(success);
			Assert::AreEqual(static_cast<unsigned char>(0xAF), static_cast<unsigned char>(result));
		}

		TEST_METHOD(FromHex_WChar_ValidMixedCase_ReturnsTrue)
		{
			constexpr DoubleChar<wchar_t> input{ L'A', L'f' };
			std::byte result;
			const bool success = Convert::FromHex(input, result);

			Assert::IsTrue(success);
			Assert::AreEqual(static_cast<unsigned char>(0xAF), static_cast<unsigned char>(result));
		}

		TEST_METHOD(FromHex_WChar_ValidDigits_ReturnsTrue)
		{
			constexpr DoubleChar<wchar_t> input{ L'9', L'9' };
			std::byte result;
			const bool success = Convert::FromHex(input, result);

			Assert::IsTrue(success);
			Assert::AreEqual(static_cast<unsigned char>(0x99), static_cast<unsigned char>(result));
		}

		TEST_METHOD(FromHex_WChar_ValidMixed_ReturnsTrue)
		{
			constexpr DoubleChar<wchar_t> input{ L'2', L'A' };
			std::byte result;
			const bool success = Convert::FromHex(input, result);

			Assert::IsTrue(success);
			Assert::AreEqual(static_cast<unsigned char>(42), static_cast<unsigned char>(result));
		}

		// FromHex Tests for wchar_t - Error cases
		TEST_METHOD(FromHex_WChar_InvalidFirstChar_ReturnsFalse)
		{
			constexpr DoubleChar<wchar_t> input{ L'G', L'0' };
			std::byte result;
			const bool success = Convert::FromHex(input, result);

			Assert::IsFalse(success);
		}

		TEST_METHOD(FromHex_WChar_InvalidSecondChar_ReturnsFalse)
		{
			constexpr DoubleChar<wchar_t> input{ L'0', L'G' };
			std::byte result;
			const bool success = Convert::FromHex(input, result);

			Assert::IsFalse(success);
		}

		TEST_METHOD(FromHex_WChar_InvalidBothChars_ReturnsFalse)
		{
			constexpr DoubleChar<wchar_t> input{ L'X', L'Y' };
			std::byte result;
			const bool success = Convert::FromHex(input, result);

			Assert::IsFalse(success);
		}

		TEST_METHOD(FromHex_WChar_InvalidSpecialChar_ReturnsFalse)
		{
			constexpr DoubleChar<wchar_t> input{ L'@', L'0' };
			std::byte result;
			const bool success = Convert::FromHex(input, result);

			Assert::IsFalse(success);
		}

		TEST_METHOD(FromHex_WChar_InvalidSpace_ReturnsFalse)
		{
			constexpr DoubleChar<wchar_t> input{ L' ', L'0' };
			std::byte result;
			const bool success = Convert::FromHex(input, result);

			Assert::IsFalse(success);
		}

		// Roundtrip Tests - char
		TEST_METHOD(Roundtrip_Char_Zero)
		{
			constexpr std::byte original{ 0x00 };
			const auto hex = Convert::ToHex<char>(original);
			std::byte result;
			const bool success = Convert::FromHex(hex, result);

			Assert::IsTrue(success);
			Assert::AreEqual(static_cast<unsigned char>(original), static_cast<unsigned char>(result));
		}

		TEST_METHOD(Roundtrip_Char_MaxValue)
		{
			constexpr std::byte original{ 0xFF };
			const auto hex = Convert::ToHex<char>(original);
			std::byte result;
			const bool success = Convert::FromHex(hex, result);

			Assert::IsTrue(success);
			Assert::AreEqual(static_cast<unsigned char>(original), static_cast<unsigned char>(result));
		}

		TEST_METHOD(Roundtrip_Char_AllValues)
		{
			for (int i = 0; i <= 255; i++)
			{
				const std::byte original{ static_cast<unsigned char>(i) };
				const auto hex = Convert::ToHex<char>(original);
				std::byte result;
				const bool success = Convert::FromHex(hex, result);

				Assert::IsTrue(success);
				Assert::AreEqual(static_cast<unsigned char>(original), static_cast<unsigned char>(result));
			}
		}

		// Roundtrip Tests - wchar_t
		TEST_METHOD(Roundtrip_WChar_Zero)
		{
			constexpr std::byte original{ 0x00 };
			const auto hex = Convert::ToHex<wchar_t>(original);
			std::byte result;
			const bool success = Convert::FromHex(hex, result);

			Assert::IsTrue(success);
			Assert::AreEqual(static_cast<unsigned char>(original), static_cast<unsigned char>(result));
		}

		TEST_METHOD(Roundtrip_WChar_MaxValue)
		{
			constexpr std::byte original{ 0xFF };
			const auto hex = Convert::ToHex<wchar_t>(original);
			std::byte result;
			const bool success = Convert::FromHex(hex, result);

			Assert::IsTrue(success);
			Assert::AreEqual(static_cast<unsigned char>(original), static_cast<unsigned char>(result));
		}

		TEST_METHOD(Roundtrip_WChar_AllValues)
		{
			for (int i = 0; i <= 255; i++)
			{
				const std::byte original{ static_cast<unsigned char>(i) };
				const auto hex = Convert::ToHex<wchar_t>(original);
				std::byte result;
				const bool success = Convert::FromHex(hex, result);

				Assert::IsTrue(success);
				Assert::AreEqual(static_cast<unsigned char>(original), static_cast<unsigned char>(result));
			}
		}

		// Boundary Tests
		TEST_METHOD(FromHex_Char_BoundaryBefore0_ReturnsFalse)
		{
			constexpr DoubleChar<char> input{ '/', '0' };
			std::byte result;
			const bool success = Convert::FromHex(input, result);

			Assert::IsFalse(success);
		}

		TEST_METHOD(FromHex_Char_BoundaryAfter9_ReturnsFalse)
		{
			constexpr DoubleChar<char> input{ ':', '0' };
			std::byte result;
			const bool success = Convert::FromHex(input, result);

			Assert::IsFalse(success);
		}

		TEST_METHOD(FromHex_Char_BoundaryBeforeA_ReturnsFalse)
		{
			constexpr DoubleChar<char> input{ '@', '0' };
			std::byte result;
			const bool success = Convert::FromHex(input, result);

			Assert::IsFalse(success);
		}

		TEST_METHOD(FromHex_Char_BoundaryAfterF_ReturnsFalse)
		{
			constexpr DoubleChar<char> input{ 'G', '0' };
			std::byte result;
			const bool success = Convert::FromHex(input, result);

			Assert::IsFalse(success);
		}

		TEST_METHOD(FromHex_Char_BoundaryBeforeLowerA_ReturnsFalse)
		{
			constexpr DoubleChar<char> input{ '`', '0' };
			std::byte result;
			const bool success = Convert::FromHex(input, result);

			Assert::IsFalse(success);
		}

		TEST_METHOD(FromHex_Char_BoundaryAfterLowerF_ReturnsFalse)
		{
			constexpr DoubleChar<char> input{ 'g', '0' };
			std::byte result;
			const bool success = Convert::FromHex(input, result);

			Assert::IsFalse(success);
		}
	};
}
