#pragma once

#include <string>

#include "combaseapi.h"

namespace Native
{
	class Guid : GUID
	{
	public:

		Guid() noexcept;

		const std::string string() const;
		const std::wstring wstring() const;

		operator const std::string() const;
		operator const std::wstring() const;

		static const Guid NewGuid();

		static const Guid Parse(const std::wstring_view value);

	private:
		Guid(const GUID& other) noexcept;
	};
}
