#pragma once

#include "EventWaitHandle.h"

namespace Native
{
	namespace Threading
	{
		class AutoResetEvent : public EventWaitHandle
		{
		public:
			AutoResetEvent(bool initialState);
		};
	}
}

