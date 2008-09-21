#include <BALL/VIEW/INPUT/buttonEvent.h>

namespace BALL
{
	namespace VIEW
	{
		ButtonEvent::ButtonEvent(EventsIDs type, InputDeviceDriver* dev, InputDeviceButton cause)
			: DeviceEvent(type, dev), cause_(cause), buttons_(cause)
		{
		}

		ButtonEvent::ButtonEvent(EventsIDs type, InputDeviceDriver* dev, InputDeviceButton cause, InputDeviceButtons buttons)
			: DeviceEvent(type, dev), cause_(cause), buttons_(buttons)
		{
		}
	}
}

