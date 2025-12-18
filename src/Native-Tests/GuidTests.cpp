#include "CppUnitTest.h"

#include "Guid.h"
#include <unordered_map>

using namespace Native;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace NativeTests
{
	TEST_CLASS(GuidTests)
	{

	public:

		TEST_METHOD(Ctor_Matches_String)
		{
			constexpr Guid guid = { 0xB463C5B8, 0x2d96, 0x4244, 0xA1F7, 0xE70746C4D2FA };

			Assert::AreEqual(std::string("B463C5B8-2D96-4244-A1F7-E70746C4D2FA"), static_cast<std::string>(guid));			
		}

		TEST_METHOD(Ctor_Matches_WSring)
		{
			constexpr Guid guid = { 0xCCDD258C, 0x2D73, 0x47EE, 0xA656, 0x11C7746EBA19 };

			Assert::AreEqual(std::wstring(L"CCDD258C-2D73-47EE-A656-11C7746EBA19"), static_cast<std::wstring>(guid));
		}

		TEST_METHOD(Parse_ThrowsExceptionForInvalidFormat)
		{
			Assert::ExpectException<std::exception>([]() { Guid::Parse(L"This is not a Guid!!!"); });
		}

		TEST_METHOD(GuidEmpty_wstring_AllZero)
		{
			Assert::AreEqual(std::wstring(L"00000000-0000-0000-0000-000000000000"), static_cast<std::wstring>(Guid::Empty));
		}

		TEST_METHOD(GuidEmpty_string_AllZero)
		{
			Assert::AreEqual(std::string("00000000-0000-0000-0000-000000000000"), static_cast<std::string>(Guid::Empty));
		}

		TEST_METHOD(EmptyGuid_wstring_AllZero)
		{
			Guid emptyGuid;

			Assert::AreEqual(std::wstring(L"00000000-0000-0000-0000-000000000000"), static_cast<std::wstring>(emptyGuid));
		}

		TEST_METHOD(EmptyGuid_string_AllZero)
		{
			Guid emptyGuid;

			Assert::AreEqual(std::string("00000000-0000-0000-0000-000000000000"), static_cast<std::string>(emptyGuid));
		}

		TEST_METHOD(Parse_string_works_with_DefaultFormat)
		{
			Guid emptyGuid = Guid::Parse("00000000-0000-0000-0000-000000000000");
		}

		TEST_METHOD(Parse_string_works_with_RegistryFormat)
		{
			Guid emptyGuid = Guid::Parse("{00000000-0000-0000-0000-000000000000}");
		}

		TEST_METHOD(Parse_wstring_works_with_DefaultFormat)
		{
			Guid emptyGuid = Guid::Parse(L"00000000-0000-0000-0000-000000000000");
		}

		TEST_METHOD(Parse_wstring_works_with_RegistryFormat)
		{
			Guid emptyGuid = Guid::Parse(L"{00000000-0000-0000-0000-000000000000}");
		}

		TEST_METHOD(Format_Guid_to_String)
		{
			// Arrange
			constexpr const Guid a_guid = GUID{ 0xe11b7641, 0x3a9b, 0x4d9e, { 0x9c, 0xb7, 0x72, 0xd8, 0x5b, 0xd, 0x81, 0xfd } }; // {E11B7641-3A9B-4D9E-9CB7-72D85B0D81FD}

			// Act
			const std::string value = std::format("{0}", a_guid);

			// Assert
			Assert::AreEqual("E11B7641-3A9B-4D9E-9CB7-72D85B0D81FD", value.c_str());
		}

		TEST_METHOD(Format_Guid_to_WString)
		{
			// Arrange
			constexpr const Guid a_guid = GUID{ 0xe127731e, 0x5991, 0x46b3, { 0xbd, 0xdd, 0x36, 0xf6, 0xa8, 0x43, 0x24, 0x85 } }; // {E127731E-5991-46B3-BDDD-36F6A8432485}

			// Act
			const std::wstring value = std::format(L"{0}", a_guid);

			// Assert
			Assert::AreEqual(L"E127731E-5991-46B3-BDDD-36F6A8432485", value.c_str());
		}

		TEST_METHOD(Hash_SameGuids_ProduceSameHash)
		{
			// Arrange
			constexpr const Guid guid1 = GUID{ 0xe11b7641, 0x3a9b, 0x4d9e, { 0x9c, 0xb7, 0x72, 0xd8, 0x5b, 0xd, 0x81, 0xfd } };
			constexpr const Guid guid2 = GUID{ 0xe11b7641, 0x3a9b, 0x4d9e, { 0x9c, 0xb7, 0x72, 0xd8, 0x5b, 0xd, 0x81, 0xfd } };

			// Act
			std::hash<Guid> hasher;
			const std::size_t hash1 = hasher(guid1);
			const std::size_t hash2 = hasher(guid2);

			// Assert
			Assert::AreEqual(hash1, hash2, L"Identical GUIDs should produce the same hash value");
		}

		TEST_METHOD(Hash_DifferentGuids_ProduceDifferentHash)
		{
			// Arrange
			constexpr const Guid guid1 = GUID{ 0xe11b7641, 0x3a9b, 0x4d9e, { 0x9c, 0xb7, 0x72, 0xd8, 0x5b, 0xd, 0x81, 0xfd } };
			constexpr const Guid guid2 = GUID{ 0xe127731e, 0x5991, 0x46b3, { 0xbd, 0xdd, 0x36, 0xf6, 0xa8, 0x43, 0x24, 0x85 } };

			// Act
			std::hash<Guid> hasher;
			const std::size_t hash1 = hasher(guid1);
			const std::size_t hash2 = hasher(guid2);

			// Assert
			Assert::AreNotEqual(hash1, hash2, L"Different GUIDs should produce different hash values");
		}

		TEST_METHOD(Hash_EmptyGuid_ProducesConsistentHash)
		{
			// Arrange
			const Guid emptyGuid1 = Guid::Empty;
			const Guid emptyGuid2 = Guid::Empty;

			// Act
			std::hash<Guid> hasher;
			const std::size_t hash1 = hasher(emptyGuid1);
			const std::size_t hash2 = hasher(emptyGuid2);

			// Assert
			Assert::AreEqual(hash1, hash2, L"Empty GUIDs should produce consistent hash values");
		}

		TEST_METHOD(Hash_WorksWithUnorderedMap)
		{
			// Arrange
			constexpr const Guid guid1 = GUID{ 0xe11b7641, 0x3a9b, 0x4d9e, { 0x9c, 0xb7, 0x72, 0xd8, 0x5b, 0xd, 0x81, 0xfd } };
			constexpr const Guid guid2 = GUID{ 0xe127731e, 0x5991, 0x46b3, { 0xbd, 0xdd, 0x36, 0xf6, 0xa8, 0x43, 0x24, 0x85 } };
			std::unordered_map<Guid, int> guidMap;

			// Act
			guidMap[guid1] = 42;
			guidMap[guid2] = 99;

			// Assert
			Assert::AreEqual(42, guidMap[guid1], L"Value for guid1 should be 42");
			Assert::AreEqual(99, guidMap[guid2], L"Value for guid2 should be 99");
			Assert::AreEqual(static_cast<std::size_t>(2), guidMap.size(), L"Map should contain exactly 2 entries");
		}

		TEST_METHOD(TryParse_string_ValidDefaultFormat_ReturnsGuid)
		{
			// Arrange
			const std::string validGuid = "E11B7641-3A9B-4D9E-9CB7-72D85B0D81FD";

			// Act
			const std::optional<Guid> result = Guid::TryParse(validGuid);

			// Assert
			Assert::IsTrue(result.has_value(), L"TryParse should return a value for valid GUID");
			Assert::AreEqual(validGuid, static_cast<std::string>(result.value()));
		}

		TEST_METHOD(TryParse_string_ValidRegistryFormat_ReturnsGuid)
		{
			// Arrange
			const std::string validGuid = "{E11B7641-3A9B-4D9E-9CB7-72D85B0D81FD}";

			// Act
			const std::optional<Guid> result = Guid::TryParse(validGuid);

			// Assert
			Assert::IsTrue(result.has_value(), L"TryParse should return a value for valid GUID with braces");
		}

		TEST_METHOD(TryParse_string_LowercaseFormat_ReturnsGuid)
		{
			// Arrange
			const std::string validGuid = "e11b7641-3a9b-4d9e-9cb7-72d85b0d81fd";

			// Act
			const std::optional<Guid> result = Guid::TryParse(validGuid);

			// Assert
			Assert::IsTrue(result.has_value(), L"TryParse should handle lowercase GUIDs");
		}

		TEST_METHOD(TryParse_string_EmptyGuid_ReturnsGuid)
		{
			// Arrange
			const std::string emptyGuid = "00000000-0000-0000-0000-000000000000";

			// Act
			const std::optional<Guid> result = Guid::TryParse(emptyGuid);

			// Assert
			Assert::IsTrue(result.has_value(), L"TryParse should parse empty GUID");
			Assert::AreEqual(emptyGuid, static_cast<std::string>(result.value()));
		}

		TEST_METHOD(TryParse_string_InvalidFormat_ReturnsNullopt)
		{
			// Arrange
			const std::string invalidGuid = "This is not a Guid!!!";

			// Act
			const std::optional<Guid> result = Guid::TryParse(invalidGuid);

			// Assert
			Assert::IsFalse(result.has_value(), L"TryParse should return nullopt for invalid GUID");
		}

		TEST_METHOD(TryParse_string_EmptyString_ReturnsNullopt)
		{
			// Arrange
			const std::string emptyString = "";

			// Act
			const std::optional<Guid> result = Guid::TryParse(emptyString);

			// Assert
			Assert::IsFalse(result.has_value(), L"TryParse should return nullopt for empty string");
		}

		TEST_METHOD(TryParse_string_TooShort_ReturnsNullopt)
		{
			// Arrange
			const std::string shortString = "E11B7641-3A9B";

			// Act
			const std::optional<Guid> result = Guid::TryParse(shortString);

			// Assert
			Assert::IsFalse(result.has_value(), L"TryParse should return nullopt for too short string");
		}

		TEST_METHOD(TryParse_string_InvalidCharacters_ReturnsNullopt)
		{
			// Arrange
			const std::string invalidChars = "GGGGGGGG-3A9B-4D9E-9CB7-72D85B0D81FD";

			// Act
			const std::optional<Guid> result = Guid::TryParse(invalidChars);

			// Assert
			Assert::IsFalse(result.has_value(), L"TryParse should return nullopt for invalid characters");
		}

		TEST_METHOD(TryParse_string_SquareBrackets_ReturnsNullopt)
		{
			// Arrange
			const std::string squareBrackets = "[E11B7641-3A9B-4D9E-9CB7-72D85B0D81FD]";

			// Act
			const std::optional<Guid> result = Guid::TryParse(squareBrackets);

			// Assert
			Assert::IsFalse(result.has_value(), L"TryParse should return nullopt for square brackets");
		}

		TEST_METHOD(TryParse_wstring_ValidDefaultFormat_ReturnsGuid)
		{
			// Arrange
			const std::wstring validGuid = L"E127731E-5991-46B3-BDDD-36F6A8432485";

			// Act
			const std::optional<Guid> result = Guid::TryParse(validGuid);

			// Assert
			Assert::IsTrue(result.has_value(), L"TryParse should return a value for valid GUID");
			Assert::AreEqual(validGuid, static_cast<std::wstring>(result.value()));
		}

		TEST_METHOD(TryParse_wstring_ValidRegistryFormat_ReturnsGuid)
		{
			// Arrange
			const std::wstring validGuid = L"{E127731E-5991-46B3-BDDD-36F6A8432485}";

			// Act
			const std::optional<Guid> result = Guid::TryParse(validGuid);

			// Assert
			Assert::IsTrue(result.has_value(), L"TryParse should return a value for valid GUID with braces");
		}

		TEST_METHOD(TryParse_wstring_LowercaseFormat_ReturnsGuid)
		{
			// Arrange
			const std::wstring validGuid = L"e127731e-5991-46b3-bddd-36f6a8432485";

			// Act
			const std::optional<Guid> result = Guid::TryParse(validGuid);

			// Assert
			Assert::IsTrue(result.has_value(), L"TryParse should handle lowercase GUIDs");
		}

		TEST_METHOD(TryParse_wstring_EmptyGuid_ReturnsGuid)
		{
			// Arrange
			const std::wstring emptyGuid = L"00000000-0000-0000-0000-000000000000";

			// Act
			const std::optional<Guid> result = Guid::TryParse(emptyGuid);

			// Assert
			Assert::IsTrue(result.has_value(), L"TryParse should parse empty GUID");
			Assert::AreEqual(emptyGuid, static_cast<std::wstring>(result.value()));
		}

		TEST_METHOD(TryParse_wstring_InvalidFormat_ReturnsNullopt)
		{
			// Arrange
			const std::wstring invalidGuid = L"This is not a Guid!!!";

			// Act
			const std::optional<Guid> result = Guid::TryParse(invalidGuid);

			// Assert
			Assert::IsFalse(result.has_value(), L"TryParse should return nullopt for invalid GUID");
		}

		TEST_METHOD(TryParse_wstring_EmptyString_ReturnsNullopt)
		{
			// Arrange
			const std::wstring emptyString = L"";

			// Act
			const std::optional<Guid> result = Guid::TryParse(emptyString);

			// Assert
			Assert::IsFalse(result.has_value(), L"TryParse should return nullopt for empty string");
		}

		TEST_METHOD(TryParse_wstring_TooShort_ReturnsNullopt)
		{
			// Arrange
			const std::wstring shortString = L"E127731E-5991";

			// Act
			const std::optional<Guid> result = Guid::TryParse(shortString);

			// Assert
			Assert::IsFalse(result.has_value(), L"TryParse should return nullopt for too short string");
		}

		TEST_METHOD(TryParse_wstring_InvalidCharacters_ReturnsNullopt)
		{
			// Arrange
			const std::wstring invalidChars = L"GGGGGGGG-5991-46B3-BDDD-36F6A8432485";

			// Act
			const std::optional<Guid> result = Guid::TryParse(invalidChars);

			// Assert
			Assert::IsFalse(result.has_value(), L"TryParse should return nullopt for invalid characters");
		}

		TEST_METHOD(TryParse_wstring_SquareBrackets_ReturnsNullopt)
		{
			// Arrange
			const std::wstring squareBrackets = L"[E127731E-5991-46B3-BDDD-36F6A8432485]";

			// Act
			const std::optional<Guid> result = Guid::TryParse(squareBrackets);

			// Assert
			Assert::IsFalse(result.has_value(), L"TryParse should return nullopt for square brackets");
		}

		TEST_METHOD(Parse_string_ValidDefaultFormat_ReturnsGuid)
		{
			// Arrange
			const std::string validGuid = "E11B7641-3A9B-4D9E-9CB7-72D85B0D81FD";

			// Act
			const Guid result = Guid::Parse(validGuid);

			// Assert
			Assert::AreEqual(validGuid, static_cast<std::string>(result));
		}

		TEST_METHOD(Parse_string_ValidRegistryFormat_ReturnsGuid)
		{
			// Arrange
			const std::string validGuid = "{E11B7641-3A9B-4D9E-9CB7-72D85B0D81FD}";

			// Act
			const Guid result = Guid::Parse(validGuid);

			// Assert
			Assert::AreEqual(std::string("E11B7641-3A9B-4D9E-9CB7-72D85B0D81FD"), static_cast<std::string>(result));
		}

		TEST_METHOD(Parse_string_LowercaseFormat_ReturnsGuid)
		{
			// Arrange
			const std::string validGuid = "e11b7641-3a9b-4d9e-9cb7-72d85b0d81fd";

			// Act
			const Guid result = Guid::Parse(validGuid);

			// Assert
			Assert::AreEqual(std::string("E11B7641-3A9B-4D9E-9CB7-72D85B0D81FD"), static_cast<std::string>(result));
		}

		TEST_METHOD(Parse_string_EmptyGuid_ReturnsGuid)
		{
			// Arrange
			const std::string emptyGuid = "00000000-0000-0000-0000-000000000000";

			// Act
			const Guid result = Guid::Parse(emptyGuid);

			// Assert
			Assert::AreEqual(emptyGuid, static_cast<std::string>(result));
		}

		TEST_METHOD(Parse_string_InvalidFormat_ThrowsException)
		{
			// Arrange
			const std::string invalidGuid = "Invalid GUID Format";

			// Act & Assert
			Assert::ExpectException<std::exception>([&invalidGuid]() { Guid::Parse(invalidGuid); });
		}

		TEST_METHOD(Parse_string_EmptyString_ThrowsException)
		{
			// Arrange
			const std::string emptyString = "";

			// Act & Assert
			Assert::ExpectException<std::exception>([&emptyString]() { Guid::Parse(emptyString); });
		}

		TEST_METHOD(Parse_string_InvalidCharacters_ThrowsException)
		{
			// Arrange
			const std::string invalidChars = "GGGGGGGG-3A9B-4D9E-9CB7-72D85B0D81FD";

			// Act & Assert
			Assert::ExpectException<std::exception>([&invalidChars]() { Guid::Parse(invalidChars); });
		}

		TEST_METHOD(Parse_string_SquareBrackets_ThrowsException)
		{
			// Arrange
			const std::string squareBrackets = "[E11B7641-3A9B-4D9E-9CB7-72D85B0D81FD]";

			// Act & Assert
			Assert::ExpectException<std::exception>([&squareBrackets]() { Guid::Parse(squareBrackets); });
		}

		TEST_METHOD(Parse_wstring_ValidDefaultFormat_ReturnsGuid)
		{
			// Arrange
			const std::wstring validGuid = L"E127731E-5991-46B3-BDDD-36F6A8432485";

			// Act
			const Guid result = Guid::Parse(validGuid);

			// Assert
			Assert::AreEqual(validGuid, static_cast<std::wstring>(result));
		}

		TEST_METHOD(Parse_wstring_ValidRegistryFormat_ReturnsGuid)
		{
			// Arrange
			const std::wstring validGuid = L"{E127731E-5991-46B3-BDDD-36F6A8432485}";

			// Act
			const Guid result = Guid::Parse(validGuid);

			// Assert
			Assert::AreEqual(std::wstring(L"E127731E-5991-46B3-BDDD-36F6A8432485"), static_cast<std::wstring>(result));
		}

		TEST_METHOD(Parse_wstring_LowercaseFormat_ReturnsGuid)
		{
			// Arrange
			const std::wstring validGuid = L"e127731e-5991-46b3-bddd-36f6a8432485";

			// Act
			const Guid result = Guid::Parse(validGuid);

			// Assert
			Assert::AreEqual(std::wstring(L"E127731E-5991-46B3-BDDD-36F6A8432485"), static_cast<std::wstring>(result));
		}

		TEST_METHOD(Parse_wstring_EmptyGuid_ReturnsGuid)
		{
			// Arrange
			const std::wstring emptyGuid = L"00000000-0000-0000-0000-000000000000";

			// Act
			const Guid result = Guid::Parse(emptyGuid);

			// Assert
			Assert::AreEqual(emptyGuid, static_cast<std::wstring>(result));
		}

		TEST_METHOD(Parse_wstring_InvalidFormat_ThrowsException)
		{
			// Arrange
			const std::wstring invalidGuid = L"Invalid GUID Format";

			// Act & Assert
			Assert::ExpectException<std::exception>([&invalidGuid]() { Guid::Parse(invalidGuid); });
		}

		TEST_METHOD(Parse_wstring_EmptyString_ThrowsException)
		{
			// Arrange
			const std::wstring emptyString = L"";

			// Act & Assert
			Assert::ExpectException<std::exception>([&emptyString]() { Guid::Parse(emptyString); });
		}

		TEST_METHOD(Parse_wstring_InvalidCharacters_ThrowsException)
		{
			// Arrange
			const std::wstring invalidChars = L"GGGGGGGG-5991-46B3-BDDD-36F6A8432485";

			// Act & Assert
			Assert::ExpectException<std::exception>([&invalidChars]() { Guid::Parse(invalidChars); });
		}

		TEST_METHOD(Parse_wstring_SquareBrackets_ThrowsException)
		{
			// Arrange
			const std::wstring squareBrackets = L"[E127731E-5991-46B3-BDDD-36F6A8432485]";

			// Act & Assert
			Assert::ExpectException<std::exception>([&squareBrackets]() { Guid::Parse(squareBrackets); });
		}
	};
}
