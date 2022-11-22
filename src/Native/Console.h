#pragma once

#include <Windows.h>

#include "Event.h"

namespace Native
{
	class ConsoleCancelEventArgs
	{
	private:
		bool _canceled = false;

	public:

		void cancel() noexcept
		{
			this->_canceled = true;
		}

		constexpr bool is_canceled() const noexcept
		{
			return this->_canceled;
		}

	};

	using ConsoleCancelEvent = Event<ConsoleCancelEventArgs>;

	class Console
	{
	private:

		static ConsoleCancelEvent::Source _CancelKeyPressEventSource;

		static bool _CtrlHandlerSetted;

		Console(); // static class

		static void CancelKeyPressEventChanged();

		static BOOL WINAPI ConsoleCtrlHandlerRoutine(DWORD dwCtrlType) noexcept;

	public:

		static const ConsoleCancelEvent CancelKeyPress;

	};
}