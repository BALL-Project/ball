#include <spaceNavigatorDriver.h>

namespace BALL
{
	namespace VIEW
	{

		SpaceNavigatorDriver::SpaceNavigatorDriver(QWidget* receiver)
			: InputDeviceDriver(receiver)
		{
		}

		int SpaceNavigatorDriver::deadzone(int x) {
			int sign = x > 0 ? 1 : -1;
			return x * sign > 5 ? x - sign * 5 : 0;
		}

		void SpaceNavigatorDriver::setEnabled(bool enabled)
		{
			InputDeviceDriver::setEnabled(enabled);

			if(enabled) {
				start();
			}
		}

		// include the correct implementation depending on the operating system used
		#if defined (BALL_COMPILER_MSVC)
			#include "spaceNavigatorDriver_win.iC"
		#elif defined (BALL_OS_DARWIN)
			#include "spaceNavigatorDriver_mac.iC"
		#else
			#include "spaceNavigatorDriver_x11.iC"
		#endif
	}
}

