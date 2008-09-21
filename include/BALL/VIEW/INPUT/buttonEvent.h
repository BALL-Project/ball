#ifndef BUTTONEVENT_H
#define BUTTONEVENT_H

#include <BALL/VIEW/KERNEL/common.h>
#include <BALL/VIEW/INPUT/deviceEvent.h>

#include <QtCore/QEvent>

namespace BALL
{
	namespace VIEW
	{
		enum InputDeviceButton {
			BUTTON1 = 1 << 0,
			BUTTON2 = 1 << 1,
			BUTTON3 = 1 << 2,
			BUTTON4 = 1 << 3,
			BUTTON5 = 1 << 4,
			BUTTON6 = 1 << 5,
			BUTTON7 = 1 << 6,
			BUTTON8 = 1 << 7,
			BUTTON9 = 1 << 8,
			BUTTON10 = 1 << 9,
			BUTTON11 = 1 << 10,
			BUTTON12 = 1 << 11
		};

		typedef QFlags<InputDeviceButton> InputDeviceButtons;

		/**
		 * This event encodes button states of a custom input device.
		 * It is emitted when a button is either pressed or released.
		 */
		class ButtonEvent : public DeviceEvent
		{
			public:
				ButtonEvent(EventsIDs type, InputDeviceDriver* dev, InputDeviceButton cause);
				ButtonEvent(EventsIDs type, InputDeviceDriver* dev, InputDeviceButton cause, InputDeviceButtons buttons);

				/**
				 * Return the button that caused the event.
				 */
				InputDeviceButton button() const { return cause_; }

				/**
				 * Returns the state of the devices buttons when the event was emitted.
				 */
				InputDeviceButtons buttons() const { return buttons_; }
			private:
				InputDeviceButton cause_;
				InputDeviceButtons buttons_;
		};

	}
}

#endif //BUTTONEVENT_H

