#include "Environment.h"

#include <Windows.h>
#include <dsgetdc.h>
#include <lm.h>
#include <shlobj_core.h>

#include <NotSupportedException.h>

#include <Win32Exception.h>
#include <HResultException.h>

namespace Native
{
	using namespace Windows;

	std::string Environment::Username()
	{
		std::string username(UNLEN, '\0');

		DWORD size = static_cast<DWORD>(username.length());

		GetUserNameA(username.data(), &size);

		username.resize(size > 0 ? size - 1 : 0);

		return username;
	}

	std::string Environment::ComputerName(const COMPUTER_NAME_FORMAT format)
	{
		DWORD size = 0;

		GetComputerNameExA(format, nullptr, &size);

		std::string computerName(size, L'\0');

		GetComputerNameExA(format, computerName.data(), &size);

		computerName.resize(size > 0 ? size : 0);

		return computerName;
	}
	std::filesystem::path Environment::GetFolderPath(const SpecialFolder folder)
	{
		int csidl;

		switch (folder)
		{
		case SpecialFolder::ApplicationData:
			csidl = CSIDL_APPDATA;
			break;
		case SpecialFolder::CommonApplicationData:
			csidl = CSIDL_COMMON_APPDATA;
			break;
		case SpecialFolder::LocalApplicationData:
			csidl = CSIDL_LOCAL_APPDATA;
			break;
		case SpecialFolder::Cookies:
			csidl = CSIDL_COOKIES;
			break;
		case SpecialFolder::Desktop:
			csidl = CSIDL_DESKTOP;
			break;
		case SpecialFolder::Favorites:
			csidl = CSIDL_FAVORITES;
			break;
		case SpecialFolder::History:
			csidl = CSIDL_HISTORY;
			break;
		case SpecialFolder::InternetCache:
			csidl = CSIDL_INTERNET_CACHE;
			break;
		case SpecialFolder::Programs:
			csidl = CSIDL_PROGRAMS;
			break;
		case SpecialFolder::MyComputer:
			csidl = CSIDL_DRIVES;
			break;
		case SpecialFolder::MyMusic:
			csidl = CSIDL_MYMUSIC;
			break;
		case SpecialFolder::MyPictures:
			csidl = CSIDL_MYPICTURES;
			break;
		case SpecialFolder::MyVideos:
			csidl = CSIDL_MYVIDEO;
			break;
		case SpecialFolder::Recent:
			csidl = CSIDL_RECENT;
			break;
		case SpecialFolder::SendTo:
			csidl = CSIDL_SENDTO;
			break;
		case SpecialFolder::StartMenu:
			csidl = CSIDL_STARTMENU;
			break;
		case SpecialFolder::Startup:
			csidl = CSIDL_STARTUP;
			break;
		case SpecialFolder::System:
			csidl = CSIDL_SYSTEM;
			break;
		case SpecialFolder::Templates:
			csidl = CSIDL_TEMPLATES;
			break;
		case SpecialFolder::DesktopDirectory:
			csidl = CSIDL_DESKTOPDIRECTORY;
			break;
		case SpecialFolder::Personal:
			csidl = CSIDL_PERSONAL;
			break;
		case SpecialFolder::MyDocuments:
			csidl = CSIDL_PERSONAL;
			break;
		case SpecialFolder::ProgramFiles:
			csidl = CSIDL_PROGRAM_FILES;
			break;
		case SpecialFolder::CommonProgramFiles:
			csidl = CSIDL_PROGRAM_FILES_COMMON;
			break;
		case SpecialFolder::AdminTools:
			csidl = CSIDL_ADMINTOOLS;
			break;
		case SpecialFolder::CDBurning:
			csidl = CSIDL_CDBURN_AREA;
			break;
		case SpecialFolder::CommonAdminTools:
			csidl = CSIDL_COMMON_ADMINTOOLS;
			break;
		case SpecialFolder::CommonDocuments:
			csidl = CSIDL_COMMON_DOCUMENTS;
			break;
		case SpecialFolder::CommonMusic:
			csidl = CSIDL_COMMON_MUSIC;
			break;
		case SpecialFolder::CommonOemLinks:
			csidl = CSIDL_COMMON_OEM_LINKS;
			break;
		case SpecialFolder::CommonPictures:
			csidl = CSIDL_COMMON_PICTURES;
			break;
		case SpecialFolder::CommonStartMenu:
			csidl = CSIDL_COMMON_STARTMENU;
			break;
		case SpecialFolder::CommonPrograms:
			csidl = CSIDL_COMMON_PROGRAMS;
			break;
		case SpecialFolder::CommonStartup:
			csidl = CSIDL_COMMON_STARTUP;
			break;
		case SpecialFolder::CommonDesktopDirectory:
			csidl = CSIDL_COMMON_DESKTOPDIRECTORY;
			break;
		case SpecialFolder::CommonTemplates:
			csidl = CSIDL_COMMON_TEMPLATES;
			break;
		case SpecialFolder::CommonVideos:
			csidl = CSIDL_COMMON_VIDEO;
			break;
		case SpecialFolder::Fonts:
			csidl = CSIDL_FONTS;
			break;
		case SpecialFolder::NetworkShortcuts:
			csidl = CSIDL_NETHOOD;
			break;
		case SpecialFolder::PrinterShortcuts:
			csidl = CSIDL_PRINTHOOD;
			break;
		case SpecialFolder::UserProfile:
			csidl = CSIDL_PROFILE;
			break;
		case SpecialFolder::CommonProgramFilesX86:
			csidl = CSIDL_PROGRAM_FILES_COMMONX86;
			break;
		case SpecialFolder::ProgramFilesX86:
			csidl = CSIDL_PROGRAM_FILESX86;
			break;
		case SpecialFolder::Resources:
			csidl = CSIDL_RESOURCES;
			break;
		case SpecialFolder::LocalizedResources:
			csidl = CSIDL_RESOURCES_LOCALIZED;
			break;
		case SpecialFolder::SystemX86:
			csidl = CSIDL_SYSTEMX86;
			break;
		case SpecialFolder::Windows:
			csidl = CSIDL_WINDOWS;
			break;
		default:
			throw NotSupportedException(std::format("The special folder {0} is not supported.", (int)folder));
		}

		char buffer[MAX_PATH];

		const HRESULT result = SHGetFolderPathA(NULL, csidl, NULL, NULL, buffer);

		if (FAILED(result))
			throw HResultException(result, nameof(SHGetFolderPathA));

		return std::filesystem::path(buffer);
	}
}
