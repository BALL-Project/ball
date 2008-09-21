#ifndef DEVICEEVENT_H
#define DEVICEEVENT_H

#include <BALL/VIEW/KERNEL/common.h>

#include <QtCore/QEvent>

namespace BALL
{
	namespace VIEW
	{
		class InputDeviceDriver;

		class DeviceEvent : public QEvent
		{
			public:
				DeviceEvent(EventsIDs type, InputDeviceDriver* device);

				InputDeviceDriver* getDevice() { return device_;}
			private:
				InputDeviceDriver* device_;
		};
	}
}

#endif //DEVICEEVENT_H

