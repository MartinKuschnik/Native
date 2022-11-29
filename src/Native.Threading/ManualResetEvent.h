#pragma once

#include "EventWaitHandle.h"

namespace Native
{
	namespace Threading
	{
		class ManualResetEvent : public EventWaitHandle
		{
		public:
			ManualResetEvent(bool initialState);
		};
	}
}

