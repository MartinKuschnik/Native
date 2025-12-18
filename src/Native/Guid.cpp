#include "Guid.h"

#include <algorithm>
#include <expected>

#include "Convert.h"
#include "FormatException.h"
#include "HResultException.h"

namespace Native
{
	using namespace Windows;

	constexpr size_t GUID_BUFFER_SIZE = 39;
	constexpr size_t GUID_LENGTH_WITHOUT_BRACKETS = 36;
	constexpr size_t GUID_LENGTH_WITH_BRACKETS = GUID_LENGTH_WITHOUT_BRACKETS + 2;

	const Guid Guid::Empty;

	template<typename TCHAR>
		requires std::same_as<TCHAR, char> || std::same_as<TCHAR, wchar_t>
	struct GuidStringLayout
	{
	public:
		struct {

			DoubleChar<TCHAR> HexPair1;
			DoubleChar<TCHAR> HexPair2;
			DoubleChar<TCHAR> HexPair3;
			DoubleChar<TCHAR> HexPair4;

		} Group1;

	private:
		TCHAR Dash1;

	public:
		struct {

			DoubleChar<TCHAR> HexPair1;
			DoubleChar<TCHAR> HexPair2;

		} Group2;

	private:
		TCHAR Dash2;

	public:
		struct {

			DoubleChar<TCHAR> HexPair1;
			DoubleChar<TCHAR> HexPair2;

		} Group3;

	private:
		TCHAR Dash3;

	public:
		struct {

			DoubleChar<TCHAR> HexPair1;
			DoubleChar<TCHAR> HexPair2;

		} Group4;

	private:
		TCHAR Dash4;

	public:
		struct {

			DoubleChar<TCHAR> HexPair1;
			DoubleChar<TCHAR> HexPair2;
			DoubleChar<TCHAR> HexPair3;
			DoubleChar<TCHAR> HexPair4;
			DoubleChar<TCHAR> HexPair5;
			DoubleChar<TCHAR> HexPair6;
		} Group5;
	};

	struct GuidBinaryLayout
	{
		struct
		{
			std::byte Byte1;
			std::byte Byte2;
			std::byte Byte3;
			std::byte Byte4;
		} Data1;

		struct
		{
			std::byte Byte1;
			std::byte Byte2;
		} Data2;

		struct
		{
			std::byte Byte1;
			std::byte Byte2;
		} Data3;

		struct
		{
			std::byte Byte1;
			std::byte Byte2;
			std::byte Byte3;
			std::byte Byte4;
			std::byte Byte5;
			std::byte Byte6;
			std::byte Byte7;
			std::byte Byte8;
		} Data4;

		constexpr operator Guid() const
		{
			return *reinterpret_cast<const Guid*>(this);
		}
	};

	enum class GuidParseError
	{
		InvalidLength,
		InvalidChar,
	};

	// Note: A unified template-based implementation was chosen to avoid code duplication between string() and wstring().
	// Initial testing revealed a performance penalty compared to separate implementations, likely due to the method's size.
	// Using __forceinline eliminates this overhead and restores performance to match duplicated implementations.
	template <typename TString>
		requires std::same_as<TString, std::string> || std::same_as<TString, std::wstring>
	__forceinline TString GuidToString(const Guid* guid)
	{
		using TChar = TString::value_type;

		TString guid_as_string(GUID_LENGTH_WITHOUT_BRACKETS, '-');

		GuidStringLayout<TChar>* const string_layout = reinterpret_cast<GuidStringLayout<TChar>*>(guid_as_string.data());

		const GuidBinaryLayout* const binary_layout = reinterpret_cast<const GuidBinaryLayout*>(guid);

		string_layout->Group1.HexPair1 = Convert::ToHex<TChar>(binary_layout->Data1.Byte4);
		string_layout->Group1.HexPair2 = Convert::ToHex<TChar>(binary_layout->Data1.Byte3);
		string_layout->Group1.HexPair3 = Convert::ToHex<TChar>(binary_layout->Data1.Byte2);
		string_layout->Group1.HexPair4 = Convert::ToHex<TChar>(binary_layout->Data1.Byte1);

		string_layout->Group2.HexPair1 = Convert::ToHex<TChar>(binary_layout->Data2.Byte2);
		string_layout->Group2.HexPair2 = Convert::ToHex<TChar>(binary_layout->Data2.Byte1);

		string_layout->Group3.HexPair1 = Convert::ToHex<TChar>(binary_layout->Data3.Byte2);
		string_layout->Group3.HexPair2 = Convert::ToHex<TChar>(binary_layout->Data3.Byte1);

		string_layout->Group4.HexPair1 = Convert::ToHex<TChar>(binary_layout->Data4.Byte1);
		string_layout->Group4.HexPair2 = Convert::ToHex<TChar>(binary_layout->Data4.Byte2);

		string_layout->Group5.HexPair1 = Convert::ToHex<TChar>(binary_layout->Data4.Byte3);
		string_layout->Group5.HexPair2 = Convert::ToHex<TChar>(binary_layout->Data4.Byte4);
		string_layout->Group5.HexPair3 = Convert::ToHex<TChar>(binary_layout->Data4.Byte5);
		string_layout->Group5.HexPair4 = Convert::ToHex<TChar>(binary_layout->Data4.Byte6);
		string_layout->Group5.HexPair5 = Convert::ToHex<TChar>(binary_layout->Data4.Byte7);
		string_layout->Group5.HexPair6 = Convert::ToHex<TChar>(binary_layout->Data4.Byte8);

		return guid_as_string;
	}

	std::string Guid::string() const
	{
		return GuidToString<std::string>(this);
	}

	std::wstring Guid::wstring() const
	{
		return GuidToString<std::wstring>(this);
	}

	Guid Guid::NewGuid()
	{
		GUID guid;
		const HRESULT hCreateGuid = CoCreateGuid(&guid);

		if (FAILED(hCreateGuid))
			throw HResultException(hCreateGuid, nameof(CoCreateGuid));

		return Guid(guid);
	}

	constexpr Native::FormatException ParseErrorToException(const GuidParseError error)
	{
		switch (error)
		{
		case GuidParseError::InvalidLength:
			return Native::FormatException("Invalid guid length.");

		case GuidParseError::InvalidChar:
			return Native::FormatException("Guid string contains invalid characters.");

		default:
			return Native::FormatException("Invalid guid format.");
		}
	}

	template<typename TStringView>
		requires std::same_as<TStringView, std::string_view> || std::same_as<TStringView, std::wstring_view>
	std::expected<Guid, GuidParseError> InternalParse(const TStringView value)
	{
		using TChar = TStringView::value_type;

		const GuidStringLayout<TChar>* str_layout;

		if (value.length() == GUID_LENGTH_WITHOUT_BRACKETS)
		{
			str_layout = reinterpret_cast<const GuidStringLayout<TChar>*>(value.data());
		}
		else if (value.length() == (GUID_LENGTH_WITH_BRACKETS))
		{
			if (value[0] != '{' || value[GUID_LENGTH_WITH_BRACKETS - 1] != '}')
				return std::unexpected(GuidParseError::InvalidChar);

			str_layout = reinterpret_cast<const GuidStringLayout<TChar>*>(value.data() + 1);
		}
		else
		{
			return std::unexpected(GuidParseError::InvalidLength);
		}

		GuidBinaryLayout binary_layout;

		if (!Convert::FromHex(str_layout->Group1.HexPair4, binary_layout.Data1.Byte1))
			return std::unexpected(GuidParseError::InvalidChar);
		if (!Convert::FromHex(str_layout->Group1.HexPair3, binary_layout.Data1.Byte2))
			return std::unexpected(GuidParseError::InvalidChar);
		if (!Convert::FromHex(str_layout->Group1.HexPair2, binary_layout.Data1.Byte3))
			return std::unexpected(GuidParseError::InvalidChar);
		if (!Convert::FromHex(str_layout->Group1.HexPair1, binary_layout.Data1.Byte4))
			return std::unexpected(GuidParseError::InvalidChar);

		if (!Convert::FromHex(str_layout->Group2.HexPair2, binary_layout.Data2.Byte1))
			return std::unexpected(GuidParseError::InvalidChar);
		if (!Convert::FromHex(str_layout->Group2.HexPair1, binary_layout.Data2.Byte2))
			return std::unexpected(GuidParseError::InvalidChar);

		if (!Convert::FromHex(str_layout->Group3.HexPair2, binary_layout.Data3.Byte1))
			return std::unexpected(GuidParseError::InvalidChar);
		if (!Convert::FromHex(str_layout->Group3.HexPair1, binary_layout.Data3.Byte2))
			return std::unexpected(GuidParseError::InvalidChar);

		if (!Convert::FromHex(str_layout->Group4.HexPair1, binary_layout.Data4.Byte1))
			return std::unexpected(GuidParseError::InvalidChar);
		if (!Convert::FromHex(str_layout->Group4.HexPair2, binary_layout.Data4.Byte2))
			return std::unexpected(GuidParseError::InvalidChar);
		if (!Convert::FromHex(str_layout->Group5.HexPair1, binary_layout.Data4.Byte3))
			return std::unexpected(GuidParseError::InvalidChar);
		if (!Convert::FromHex(str_layout->Group5.HexPair2, binary_layout.Data4.Byte4))
			return std::unexpected(GuidParseError::InvalidChar);
		if (!Convert::FromHex(str_layout->Group5.HexPair3, binary_layout.Data4.Byte5))
			return std::unexpected(GuidParseError::InvalidChar);
		if (!Convert::FromHex(str_layout->Group5.HexPair4, binary_layout.Data4.Byte6))
			return std::unexpected(GuidParseError::InvalidChar);
		if (!Convert::FromHex(str_layout->Group5.HexPair5, binary_layout.Data4.Byte7))
			return std::unexpected(GuidParseError::InvalidChar);
		if (!Convert::FromHex(str_layout->Group5.HexPair6, binary_layout.Data4.Byte8))
			return std::unexpected(GuidParseError::InvalidChar);

		return binary_layout;
	}

	Guid Guid::Parse(const std::string_view value)
	{
		const std::expected<Guid, GuidParseError> result = InternalParse(value);

		if (result)
			return *result;

		throw ParseErrorToException(result.error());
	}

	Guid Guid::Parse(const std::wstring_view value)
	{
		const std::expected<Guid, GuidParseError> result = InternalParse(value);

		if (result)
			return *result;

		throw ParseErrorToException(result.error());
	}

	std::optional<Guid> Guid::TryParse(const std::string_view value)
	{
		const std::expected<Guid, GuidParseError> result = InternalParse(value);

		if (result)
			return *result;

		return std::nullopt;
	}

	std::optional<Guid> Guid::TryParse(const std::wstring_view value)
	{
		const std::expected<Guid, GuidParseError> result = InternalParse(value);

		if (result)
			return *result;

		return std::nullopt;
	}

	Guid::operator std::string() const
	{
		return this->string();
	}

	Guid::operator std::wstring() const
	{
		return this->wstring();
	}
}