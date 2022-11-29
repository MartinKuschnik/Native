#include "ManualResetEvent.h"

namespace Native
{
	namespace Threading
	{
		ManualResetEvent::ManualResetEvent(bool initialState)
			: EventWaitHandle(initialState, EventResetMode::ManualReset)
		{
		}
	}
}