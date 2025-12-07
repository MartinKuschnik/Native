#pragma once

#include <filesystem>
#include <concepts>

#include <Windows.h>

#include "LoadLibraryOption.h"

namespace Native
{
	class Library
	{
	public:
		Library(const Library&) = delete;

		Library(Library&&) noexcept;

		virtual ~Library() noexcept;

		Library& operator=(Library&& other) noexcept;

		template<typename T>
			requires std::is_pointer_v<T>&& std::is_function_v<std::remove_pointer_t<T>>
		constexpr T get_exported_function(const std::string_view function_name)
		{
			return reinterpret_cast<T>(this->get_exported_function(function_name));
		}

		static Library Load(const std::filesystem::path path, const LoadLibraryOption options = LoadLibraryOption::LoadLibrarySearchSystem32);

	private:
		Library(HMODULE hLib) noexcept;

		FARPROC get_exported_function(const std::string_view function_name);

		HMODULE _hLib;
	};
}
