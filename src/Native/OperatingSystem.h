#pragma once

#include <format>

#include "PlatformID.h"
#include "Version.h"

namespace Native
{
	class OperatingSystem
	{
	public:
		const PlatformID Platform;
		const Version Version;

#if _WIN32
		const bool IsWindowsServer;
		const std::wstring Edition;


		constexpr OperatingSystem(const PlatformID platform, const Native::Version version, const bool is_windows_server, const std::wstring edition) noexcept
			: Platform(platform), Version(version), IsWindowsServer(is_windows_server), Edition(edition)
		{
		}

#elif
		constexpr OperatingSystem(PlatformID platform, Native::Version version) noexcept
			: Platform(platform), Version(version)
		{
		}
#endif

		std::wstring wstring() const;

		static bool IsWindowsVersionAtLeast(const uint32_t major, const uint32_t minor = 0, const uint32_t build = 0, const uint32_t revision = 0);
		static bool IsWindowsVersionAtLeast(const OperatingSystem& os, const uint32_t major, const uint32_t minor = 0, const uint32_t build = 0, const uint32_t revision = 0);

	private:

		static constexpr bool IsWindows()
		{
#if _WIN32
			return true;
#elif
			return false;
#endif
		}

		static bool IsOSVersionAtLeast(const uint32_t major, const uint32_t minor, const uint32_t build, const uint32_t revision);
		static bool IsOSVersionAtLeast(const OperatingSystem& os, const uint32_t major, const uint32_t minor, const uint32_t build, const uint32_t revision);
	};
}

template <>
struct std::formatter<Native::OperatingSystem, wchar_t> : std::formatter<wstring_view, wchar_t>
{
	auto format(const Native::OperatingSystem& os, std::wformat_context& ctx) const 
	{
		return std::formatter<wstring_view, wchar_t>::format(os.wstring(), ctx);
	}
};