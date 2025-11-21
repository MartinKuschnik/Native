#pragma once

#include <string>

namespace Native
{
	namespace Web
	{
		class HttpUtility
		{
		public:
			HttpUtility() = delete; // static class

			static std::wstring UrlDecode(const std::wstring_view url_encoded_string);
		};
	}
}