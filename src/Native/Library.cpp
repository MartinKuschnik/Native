#include "Library.h"

#include "Win32Exception.h"

namespace Native
{
	using namespace Windows;

	Library::Library(HMODULE hLib) noexcept
		: _hLib(hLib)
	{
	}

	Library::Library(Library&& other) noexcept
		: _hLib(other._hLib)
	{
		other._hLib = NULL;
	}

	Library::~Library() noexcept
	{
		if (this->_hLib != NULL)
		{
			FreeLibrary(this->_hLib);
			this->_hLib = NULL;
		}
	}

	Library& Library::operator=(Library&& other) noexcept
	{
		if (this->_hLib != NULL)
			FreeLibrary(this->_hLib);

		this->_hLib = other._hLib;
		other._hLib = NULL;

		return *this;
	}

	FARPROC Library::get_exported_function(const std::string_view function_name)
	{
		const FARPROC proc = GetProcAddress(this->_hLib, function_name.data());

		if (proc == nullptr)
			throw Windows::Win32Exception(GetLastError(), nameof(GetProcAddress));

		return proc;
	}

	Library Library::Load(const std::filesystem::path path, const LoadLibraryOption options)
	{
		HMODULE hLib = LoadLibraryExW(path.c_str(), NULL, static_cast<DWORD>(options));

		if (hLib == NULL)
			throw Win32Exception(GetLastError(), nameof(LoadLibraryExW));

		return Library(hLib);
	}
}
