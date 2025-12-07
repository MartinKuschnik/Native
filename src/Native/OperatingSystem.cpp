#include "OperatingSystem.h"

#include "Environment.h"

#include "NotSupportedException.h"

namespace Native
{
	bool OperatingSystem::IsWindowsVersionAtLeast(const uint32_t major, const uint32_t minor, const uint32_t build, const uint32_t revision)
	{
		return IsWindows() && IsOSVersionAtLeast(major, minor, build, revision);
	}

    bool OperatingSystem::IsWindowsVersionAtLeast(const OperatingSystem& os, const uint32_t major, const uint32_t minor, const uint32_t build, const uint32_t revision)
    {
        return IsWindows() && IsOSVersionAtLeast(os, major, minor, build, revision);
    }

    std::wstring OperatingSystem::wstring() const
    {
#if _WIN32

        if (this->IsWindowsServer)
        {
            if (this->Version.Build >= 26100)
                return std::format(L"Windows Server 2025 ({}.{}.{}.{})", this->Version.Major, this->Version.Minor, this->Version.Build, this->Version.Revision);
            else if (this->Version.Build >= 20348)
                return std::format(L"Windows Server 2022 ({}.{}.{}.{})", this->Version.Major, this->Version.Minor, this->Version.Build, this->Version.Revision);
            else if (this->Version.Build >= 17763)
                return std::format(L"Windows Server 2019 ({}.{}.{}.{})", this->Version.Major, this->Version.Minor, this->Version.Build, this->Version.Revision);
            else if (this->Version.Build >= 14393)
                return std::format(L"Windows Server 2016 ({}.{}.{}.{})", this->Version.Major, this->Version.Minor, this->Version.Build, this->Version.Revision);
            else
                return std::format(L"Windows Server ({}.{}.{}.{})", this->Version.Major, this->Version.Minor, this->Version.Build, this->Version.Revision);
        }
        else
        {
            if (this->Version.Build >= 22000)
                return std::format(L"Windows 11 {} ({}.{}.{}.{})", this->Edition, this->Version.Major, this->Version.Minor, this->Version.Build, this->Version.Revision);
            else if (this->Version.Build >= 19041)
                return std::format(L"Windows 10 {} ({}.{}.{}.{})", this->Edition, this->Version.Major, this->Version.Minor, this->Version.Build, this->Version.Revision);
            else
                return std::format(L"Windows {} ({}.{}.{}.{})", this->Edition, this->Version.Major, this->Version.Minor, this->Version.Build, this->Version.Revision);
        }
#else
        throw NotSupportedException("Currently only Windows is supported.");

#endif // _WIN32

    }

	bool OperatingSystem::IsOSVersionAtLeast(const uint32_t major, const uint32_t minor, const uint32_t build, const uint32_t revision)
	{
        return IsOSVersionAtLeast(Environment::GetOSVersion(), major, minor, build, revision);
	}

    bool OperatingSystem::IsOSVersionAtLeast(const OperatingSystem& os, const uint32_t major, const uint32_t minor, const uint32_t build, const uint32_t revision)
    {
        const Native::Version& version = os.Version;

        if (version.Major != major)
            return version.Major > major;

        if (version.Minor != minor)
            return version.Minor > minor;

        if (version.Build != build)
            return version.Build > build;

        return version.Revision >= revision;
    }
}
