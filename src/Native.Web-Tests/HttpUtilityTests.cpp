#include "CppUnitTest.h"

#include "HttpUtility.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace Native::Web;

namespace NativeWebTests
{
	TEST_CLASS(HttpUtilityTests)
	{
	public:
		
        TEST_METHOD(UrlDecode_SimplePercentEncoding)
        {
            std::wstring input = L"Hello%20World";
            auto result = HttpUtility::UrlDecode(input);
            Assert::AreEqual(L"Hello World", result.c_str());
        }

        TEST_METHOD(UrlDecode_MultipleEncodedParts)
        {
            std::wstring input = L"https%3A%2F%2Fexample.com%2Ftest%3Fid%3D1";
            auto result = HttpUtility::UrlDecode(input);
            Assert::AreEqual(L"https://example.com/test?id=1", result.c_str());
        }

        TEST_METHOD(UrlDecode_PlusAsSpace)
        {
            std::wstring input = L"Ein+Test+Wert";
            auto result = HttpUtility::UrlDecode(input);
            Assert::AreEqual(L"Ein Test Wert", result.c_str());
        }

        TEST_METHOD(UrlDecode_EmptyString)
        {
            std::wstring input = L"";
            auto result = HttpUtility::UrlDecode(input);
            Assert::AreEqual(L"", result.c_str());
        }

        TEST_METHOD(UrlDecode_InvalidEncoding_IgnoresInvalid)
        {
            // "%2" endet unvollständig — Implementationen lassen dies meist unverändert stehen
            std::wstring input = L"Test%2GString%";
            auto result = HttpUtility::UrlDecode(input);
            Assert::AreEqual(L"Test%2GString%", result.c_str());
        }

        TEST_METHOD(UrlDecode_TrailingPercent)
        {
            std::wstring input = L"Test%";
            auto result = HttpUtility::UrlDecode(input);
            Assert::AreEqual(L"Test%", result.c_str());
        }

        TEST_METHOD(UrlDecode_SingleHexDigit)
        {
            std::wstring input = L"Value%A";
            auto result = HttpUtility::UrlDecode(input);
            Assert::AreEqual(L"Value%A", result.c_str());
        }

        TEST_METHOD(UrlDecode_InvalidHexChars)
        {
            std::wstring input = L"%Z1Test";
            auto result = HttpUtility::UrlDecode(input);
            Assert::AreEqual(L"%Z1Test", result.c_str());
        }

        TEST_METHOD(UrlDecode_HexCaseInsensitive)
        {
            std::wstring inputUpper = L"%3A";  // :
            std::wstring inputLower = L"%3a";  // :

            Assert::AreEqual(L":", HttpUtility::UrlDecode(inputUpper).c_str());
            Assert::AreEqual(L":", HttpUtility::UrlDecode(inputLower).c_str());
        }

        TEST_METHOD(UrlDecode_LongEncodedSequence)
        {
            std::wstring input;
            for (int i = 0; i < 100; ++i)
                input += L"%41";  // "A"

            auto result = HttpUtility::UrlDecode(input);
            Assert::AreEqual(std::wstring(100, L'A'), result);
        }

        TEST_METHOD(UrlDecode_SpecialUrlCharacters)
        {
            std::wstring input = L"%3B%3D%23%3F%26";
            auto result = HttpUtility::UrlDecode(input);
            Assert::AreEqual(L";=#?&", result.c_str());
        }

        TEST_METHOD(UrlDecode_PlusInsideEncodedValue)
        {
            // "%2B" ist "+"
            std::wstring input = L"Value%2BMore";
            auto result = HttpUtility::UrlDecode(input);
            Assert::AreEqual(L"Value+More", result.c_str());
        }
	};
}
