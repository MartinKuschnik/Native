#pragma once

#include <cstdint>

namespace Native
{
	class Version
	{
	public:

		const uint32_t Major;
		const uint32_t Minor;
		const uint32_t Build;
		const uint32_t Revision;

		constexpr Version(const uint32_t major, const uint32_t minor, const uint32_t build, const uint32_t revision) noexcept
			: Major(major), Minor(minor), Build(build), Revision(revision)
		{
		}
	};
}