#pragma once

namespace Native
{
	namespace Net
	{
		class WsaGuard
		{
		public:
			WsaGuard();
			WsaGuard(const WsaGuard& other);
			WsaGuard(WsaGuard&& other) noexcept;
			~WsaGuard();
		private:
			bool _moved = false;
		};
	}
}
