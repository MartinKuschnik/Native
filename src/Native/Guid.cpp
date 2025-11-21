#include "Guid.h"

#include <algorithm>

#include "HResultException.h"

namespace Native
{
	using namespace Windows;

	constexpr size_t GUID_BUFFER_SIZE = 39;
	constexpr size_t GUID_LENGTH_WITHOUT_BRACKETS = 36;

	const Guid Guid::Empty;

	const std::string Guid::string() const
	{
		wchar_t lpsz[GUID_BUFFER_SIZE];

		// Does not check the return value because the function can only fail if the buffer is to small
		// and this should not be possible because we defined it by a constant.
		int result = StringFromGUID2(*this, &lpsz[0], GUID_BUFFER_SIZE);

		const std::wstring_view asWString(&lpsz[1], GUID_LENGTH_WITHOUT_BRACKETS);

		std::string asString(GUID_LENGTH_WITHOUT_BRACKETS, 0);

		std::transform(
			asWString.begin(),
			asWString.end(),
			asString.begin(),
			[](wchar_t c) { return (char)c; }
		);

		return asString;
	}

	const std::wstring Guid::wstring() const
	{
		wchar_t lpsz[GUID_BUFFER_SIZE];

		// Does not check the return value because the function can only fail if the buffer is to small
		// and this should not be possible because we defined it by a constant.
		int result = StringFromGUID2(*this, &lpsz[0], GUID_BUFFER_SIZE);

		return std::wstring(&lpsz[1], GUID_LENGTH_WITHOUT_BRACKETS);
	}

	const Guid Guid::NewGuid()
	{
		GUID guid;
		const HRESULT hCreateGuid = CoCreateGuid(&guid);

		if (FAILED(hCreateGuid))
			throw HResultException(hCreateGuid, nameof(CoCreateGuid));

		return Guid(guid);
	}

	const Guid Guid::Parse(const std::string_view value)
	{
		std::wstring wValue(value.begin(), value.end());

		// ToDo: find a more efficient way
		if (wValue.length() == 36)
		{
			wValue.insert(wValue.begin(), '{');
			wValue.insert(wValue.end(), '}');
		}

		GUID guid;

		const HRESULT hCreateGuid = CLSIDFromString(wValue.data(), &guid);

		if (FAILED(hCreateGuid))
			throw HResultException(hCreateGuid, nameof(CLSIDFromString));

		return Guid(guid);
	}

	const Guid Guid::Parse(const std::wstring_view value)
	{
		GUID guid;
		HRESULT hCreateGuid;

		// ToDo: find a more efficient way
		if (value.length() == 36)
		{
			std::wstring padded(value);
			padded.insert(padded.begin(), 1, L'{');
			padded.insert(padded.end(), 1, '}');

			hCreateGuid = CLSIDFromString(padded.data(), &guid);
		}
		else
		{
			hCreateGuid = CLSIDFromString(value.data(), &guid);
		}

		if (FAILED(hCreateGuid))
			throw HResultException(hCreateGuid, nameof(CLSIDFromString));

		return Guid(guid);
	}

	const std::optional<Guid> Guid::TryParse(const std::string_view value)
	{
		std::wstring wValue(value.begin(), value.end());

		// ToDo: find a more efficient way
		if (wValue.length() == 36)
		{
			wValue.insert(wValue.begin(), '{');
			wValue.insert(wValue.end(), '}');
		}

		GUID guid;

		const HRESULT hCreateGuid = CLSIDFromString(wValue.data(), &guid);

		if (FAILED(hCreateGuid))
		{
			if (hCreateGuid == CO_E_CLASSSTRING)
				return std::nullopt;

			throw HResultException(hCreateGuid, nameof(CLSIDFromString));
		}

		return Guid(guid);
	}

	const std::optional<Guid> Guid::TryParse(const std::wstring_view value)
	{
		GUID guid;
		HRESULT hCreateGuid;

		// ToDo: find a more efficient way
		if (value.length() == 36)
		{
			std::wstring padded(value);
			padded.insert(padded.begin(), 1, L'{');
			padded.insert(padded.end(), 1, '}');

			hCreateGuid = CLSIDFromString(padded.data(), &guid);
		}
		else
		{
			hCreateGuid = CLSIDFromString(value.data(), &guid);
		}

		if (FAILED(hCreateGuid))
		{
			if (hCreateGuid == CO_E_CLASSSTRING)
				return std::nullopt;

			throw HResultException(hCreateGuid, nameof(CLSIDFromString));
		}

		return Guid(guid);
	}

	Guid::operator const std::string() const
	{
		return this->string();
	}

	Guid::operator const std::wstring() const
	{
		return this->wstring();
	}
}