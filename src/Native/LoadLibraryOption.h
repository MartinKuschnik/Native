#pragma once

#include <type_traits>

namespace Native
{
	/// <summary>
	/// Specifies load options for Library::Load(...) function.
	/// </summary>
	enum class LoadLibraryOption
	{
		/// <summary>
		/// If this value is used, the system does not call DllMain when the DLL is loaded or unloaded.
		/// This also affects the behavior of the loader regarding references exported by the loaded DLL.
		/// </summary>
		DontResolveDllReferences = 0x00000001,

		/// <summary>
		/// The DLL is loaded as if it were a data file. Nothing is done to execute or process the DLL.
		/// This is useful for error messages.
		/// </summary>
		LoadLibraryAsDatafile = 0x00000002,

		/// <summary>
		/// The DLL is loaded using the altered search path specified in the lpFileName parameter.
		/// </summary>
		LoadWithAlteredSearchPath = 0x00000008,

		/// <summary>
		/// Ignore the code integrity level of the image.
		/// </summary>
		LoadIgnoreCodeAuthzLevel = 0x00000010,

		/// <summary>
		/// The DLL is loaded as an image resource. This can be used to load 32-bit DLLs and execute code from them,
		/// even if you are running on a 64-bit system, or vice versa.
		/// </summary>
		LoadLibraryAsImageResource = 0x00000020,

		/// <summary>
		/// Similar to LoadLibraryAsDatafile, but with exclusive access. Only the calling process can access the loaded resource.
		/// </summary>
		LoadLibraryAsDatafileExclusive = 0x00000040,

		/// <summary>
		/// The DLL is loaded only if it is digitally signed. If the file does not meet this requirement,
		/// the function fails and returns NULL.
		/// </summary>
		LoadLibraryRequireSignedTarget = 0x00000080,

		/// <summary>
		/// Search path group: If the DLL being loaded is a pure managed assembly, the search path is restricted to the directory containing the DLL being loaded.
		/// </summary>
		LoadLibrarySearchDllLoadDir = 0x00000100,

		/// <summary>
		/// Search path group: The directory of the application is searched.
		/// </summary>
		LoadLibrarySearchApplicationDir = 0x00000200,

		/// <summary>
		/// Search path group: User directories are searched.
		/// </summary>
		LoadLibrarySearchUserDirs = 0x00000400,

		/// <summary>
		/// Search path group: The System32 directory is searched.
		/// </summary>
		LoadLibrarySearchSystem32 = 0x00000800,

		/// <summary>
		/// Search path group: Equivalent to combining LoadLibrarySearchApplicationDir, LoadLibrarySearchSystem32,
		/// LoadLibrarySearchUserDirs, and LoadLibrarySearchDllLoadDir.
		/// </summary>
		LoadLibrarySearchDefaultDirs = 0x00001000,

		/// <summary>
		/// The DLL is loaded with a safe current directory. This prevents DLL preloading attacks.
		/// </summary>
		LoadLibrarySafeCurrentDirs = 0x00002000,

		/// <summary>
		/// Search path group: System32 directory is searched but DLL forwarding is disabled.
		/// </summary>
		LoadLibrarySearchSystem32NoForwarder = 0x00004000,

		/// <summary>
		/// The DLL is loaded with OS integrity continuity enabled. This is used for secure boot scenarios.
		/// </summary>
		LoadLibraryOsIntegrityContinuity = 0x00008000
	};

	constexpr LoadLibraryOption operator &(const LoadLibraryOption lhs, const LoadLibraryOption rhs) {
		using T = std::underlying_type_t<LoadLibraryOption>;
		return static_cast<LoadLibraryOption>(static_cast<T>(lhs) & static_cast<T>(rhs));
	}

	constexpr LoadLibraryOption operator |(const LoadLibraryOption lhs, const LoadLibraryOption rhs) {
		using T = std::underlying_type_t<LoadLibraryOption>;
		return static_cast<LoadLibraryOption>(static_cast<T>(lhs) | static_cast<T>(rhs));
	}

	constexpr LoadLibraryOption operator^(const LoadLibraryOption lhs, const LoadLibraryOption rhs) {
		using T = std::underlying_type_t<LoadLibraryOption>;
		return static_cast<LoadLibraryOption>(static_cast<T>(lhs) ^ static_cast<T>(rhs));
	}

	constexpr LoadLibraryOption operator~(const LoadLibraryOption f) {
		using T = std::underlying_type_t<LoadLibraryOption>;
		return static_cast<LoadLibraryOption>(~static_cast<T>(f));
	}
}