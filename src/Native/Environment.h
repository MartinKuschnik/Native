#pragma once

#include <Windows.h>

#include <string>
#include <filesystem>

#include "OperatingSystem.h"

namespace Native
{
	class Environment
	{
	public:

		enum class SpecialFolder
		{
			ApplicationData,
			CommonApplicationData,
			LocalApplicationData,
			Cookies,
			Desktop,
			Favorites,
			History,
			InternetCache,
			Programs,
			MyComputer,
			MyMusic,
			MyPictures,
			MyVideos,
			Recent,
			SendTo,
			StartMenu,
			Startup,
			System,
			Templates,
			DesktopDirectory,
			Personal,
			MyDocuments,
			ProgramFiles,
			CommonProgramFiles,
			AdminTools,
			CDBurning,
			CommonAdminTools,
			CommonDocuments,
			CommonMusic,
			CommonOemLinks,
			CommonPictures,
			CommonStartMenu,
			CommonPrograms,
			CommonStartup,
			CommonDesktopDirectory,
			CommonTemplates,
			CommonVideos,
			Fonts,
			NetworkShortcuts,
			PrinterShortcuts,
			UserProfile,
			CommonProgramFilesX86,
			ProgramFilesX86,
			Resources,
			LocalizedResources,
			SystemX86,
			Windows,
		};

	public:
		Environment() = delete;

		static std::string Username();

		static std::string ComputerName(const COMPUTER_NAME_FORMAT format);

		static std::filesystem::path GetFolderPath(const SpecialFolder folder);

		static OperatingSystem GetOSVersion();
	};
}