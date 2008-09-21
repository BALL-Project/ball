#include <BALL/VIEW/INPUT/deviceEvent.h>

namespace BALL
{
	namespace VIEW
	{
		DeviceEvent::DeviceEvent(EventsIDs type, InputDeviceDriver* device)
			: QEvent(static_cast<QEvent::Type>(type)), device_(device)
		{
		}
	}
}

