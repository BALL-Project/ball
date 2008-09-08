#include <BALL/VIEW/INPUT/inputDeviceDriver.h>

namespace BALL
{
	namespace VIEW
	{
		class FakeDriver : public InputDeviceDriver, protected QThread
		{
			public:
				FakeDriver(Scene* receiver)
					: InputDeviceDriver(receiver) {}
				bool setUp() { return true; }
				bool tearDown() { return true; }

				void setEnabled(bool enabled) { InputDeviceDriver::setEnabled(enabled); if(enabled) start(); }

			private:
				void run() { while(true) { emitPositionChange(1,2,-3,4,5,6); msleep(20); } }
		};
	}
}
