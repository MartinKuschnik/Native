#pragma once

#include <string>

#include "combaseapi.h"

namespace Native
{
	class Guid : public GUID
	{
	public:

		Guid() noexcept;
		Guid(const Guid& other) noexcept;

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

namespace std {

	template <>
	struct hash<Native::Guid>
	{
		// ToDo: add unit tests for hash<Native::Guid>
		std::size_t operator()(const Native::Guid& guid) const
		{
			const std::uint64_t* p = reinterpret_cast<const std::uint64_t*>(&guid);
			std::hash<std::uint64_t> hash;
			return hash(p[0]) ^ hash(p[1]);
		}
	};

}
