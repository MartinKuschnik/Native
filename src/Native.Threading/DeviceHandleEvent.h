#pragma once

#include "Handle.h"
#include "Guid.h"
#include "WaitHandle.h"
#include "ManualResetEvent.h"

#include <cfgmgr32.h>
#include <initguid.h>
#include <ioevent.h>

namespace Native
{
	namespace Threading
	{
		class DeviceHandleEvent : public WaitHandle
		{
		private:

			const Guid _eventId;

			ManualResetEvent _manualReseEvent;

			HCMNOTIFICATION _notification;

		public:

			DeviceHandleEvent(const Windows::Handle& device_hanlde, const Guid event_id);

			DeviceHandleEvent() = delete;
			DeviceHandleEvent(const DeviceHandleEvent&) = delete;
			DeviceHandleEvent(DeviceHandleEvent&&) = delete;

			virtual ~DeviceHandleEvent() noexcept;

		protected:

			virtual uint16_t count_handles() const override;
			virtual uint16_t copy_handles(HandleArray& dest, const uint16_t index) const override;

		private:

			static DWORD NotificationCallback(HCMNOTIFICATION hNotify, PVOID Context, CM_NOTIFY_ACTION Action, PCM_NOTIFY_EVENT_DATA EventData, DWORD EventDataSize) noexcept;

		};
	}
}

