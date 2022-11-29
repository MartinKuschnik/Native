#include "AutoResetEvent.h"

namespace Native
{
	namespace Threading
	{
		AutoResetEvent::AutoResetEvent(bool initialState)
			: EventWaitHandle(initialState, EventResetMode::AutoReset)
		{
		}
	}
}