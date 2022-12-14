#include "Console.h"

#include "Win32Exception.h"

using namespace Native::Windows;

namespace Native
{
	bool Console::_CtrlHandlerSetted = false;

	const ConsoleCancelEvent Console::CancelKeyPress(&Console::CancelKeyPressEventChanged);

	BOOL WINAPI Console::ConsoleCtrlHandlerRoutine(DWORD dwCtrlType) noexcept
	{
		ConsoleCancelEventArgs args;

		switch (dwCtrlType)
		{
			case CTRL_C_EVENT:

				Console::CancelKeyPress(args);

				// Signal is handled - don't pass it on to the next handler
				return args.is_canceled() ? TRUE : FALSE;
			default:
				// Pass signal on to the next handler
				return FALSE;
		}
	}

	void Console::CancelKeyPressEventChanged()
	{
		if (CancelKeyPress.has_subscribers())
		{
			if (!Console::_CtrlHandlerSetted)
			{
				BOOL succ = SetConsoleCtrlHandler(ConsoleCtrlHandlerRoutine, TRUE);

				if (!succ)
					throw Win32Exception(GetLastError(), nameof(SetConsoleCtrlHandler));

				Console::_CtrlHandlerSetted = true;
			}
		}
		else if (Console::_CtrlHandlerSetted)
		{
			BOOL succ = SetConsoleCtrlHandler(ConsoleCtrlHandlerRoutine, FALSE);

			if (!succ)
				throw Win32Exception(GetLastError(), nameof(SetConsoleCtrlHandler));

			Console::_CtrlHandlerSetted = false;
		}
	}
}