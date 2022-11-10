#include "Console.h"

namespace Native
{
	EventSource<ConsoleCancelEventArgs> Console::CancelKeyPressEventSource = EventSource<ConsoleCancelEventArgs>();
	
	const Event<ConsoleCancelEventArgs> Console::CancelKeyPress = Console::CancelKeyPressEventSource.create_event();
}