#include <BALL/VIEW/INPUT/spaceNavigatorDriver.h>

#include <spnav.h>

namespace BALL
{
	namespace VIEW
	{

		SpaceNavigatorDriver::SpaceNavigatorDriver(Scene* receiver)
			: InputDeviceDriver(receiver)
		{
		}

		void SpaceNavigatorDriver::run()
		{
		spnav_event sev;
		bool drop=false;

			while(isEnabled()) {
				if(drop) {
					spnav_remove_events(SPNAV_EVENT_MOTION);
					drop=false;
					continue;
				}

				if(spnav_poll_event(&sev) && (sev.type == SPNAV_EVENT_MOTION)) {
					printf("got motion event: t(%d, %d, %d) ", sev.motion.x, sev.motion.y, sev.motion.z);
					printf("r(%d, %d, %d)\n", sev.motion.rx, sev.motion.ry, sev.motion.rz);

					emitPositionChange(sev.motion.x, sev.motion.y, sev.motion.z,
					                   sev.motion.rx, sev.motion.ry, sev.motion.rz);

					drop=true;
				}

				msleep(10);
			}
		}

		bool SpaceNavigatorDriver::setUp()
		{
			return spnav_open() != -1;
		}

		bool SpaceNavigatorDriver::tearDown()
		{
			spnav_close();

			return true;
		}

		void SpaceNavigatorDriver::setEnabled(bool enabled)
		{
			InputDeviceDriver::setEnabled(enabled);

			if(enabled) {
				start();
			}
		}

	}
}

