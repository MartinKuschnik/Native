#include "DeviceHandleEvent.h"

#include <Util.h>
#include <ConfigurationManagerReturnCodeException.h>

namespace Native
{
	using namespace Windows;

	namespace Threading
	{
		DeviceHandleEvent::DeviceHandleEvent(const Windows::Handle& device_hanlde, const Guid event_id)
			: _manualReseEvent(false),
			_eventId(event_id)
		{
			CM_NOTIFY_FILTER filter = { };
			filter.cbSize = sizeof(CM_NOTIFY_FILTER);
			filter.FilterType = CM_NOTIFY_FILTER_TYPE_DEVICEHANDLE;
			filter.u.DeviceHandle.hTarget = device_hanlde;

			CONFIGRET result = ::CM_Register_Notification(&filter, this, (PCM_NOTIFY_CALLBACK)&NotificationCallback, &this->_notification);

			if (result != CR_SUCCESS)
				throw ConfigurationManagerReturnCodeException(result, nameof(CM_Register_Notification));
		}

		DeviceHandleEvent::~DeviceHandleEvent()
		{
			::CM_Unregister_Notification(_notification);
		}

		uint16_t DeviceHandleEvent::count_handles() const
		{
			return WaitHandle::CountHandles(this->_manualReseEvent);
		}

		uint16_t DeviceHandleEvent::copy_handles(HandleArray& dest, const uint16_t index) const
		{
			return WaitHandle::CopyHandles(this->_manualReseEvent, dest, index);
		}

		DWORD DeviceHandleEvent::NotificationCallback(HCMNOTIFICATION hNotify, PVOID Context, CM_NOTIFY_ACTION Action, PCM_NOTIFY_EVENT_DATA EventData, DWORD EventDataSize) noexcept
		{
			DeviceHandleEvent* _this = reinterpret_cast<Native::Threading::DeviceHandleEvent*>(Context);

			if (EventData->u.DeviceHandle.EventGuid == _this->_eventId)
				_this->_manualReseEvent.set();

			return ERROR_SUCCESS;
		}
	}
}