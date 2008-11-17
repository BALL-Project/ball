#include <spaceNavigatorDriver.h>

#include <spnav.h>

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
			return x * sign > 50 ? x - sign * 50 : 0;
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
					printf("got motion event: t(%d, %d, %d) ", deadzone(sev.motion.x), deadzone(sev.motion.y), deadzone(sev.motion.z));
					printf("r(%d, %d, %d)\n", deadzone(sev.motion.rx), deadzone(sev.motion.ry), deadzone(sev.motion.rz));

					emitPositionChange( deadzone(sev.motion.x), deadzone(sev.motion.y), deadzone(sev.motion.z),
					                    deadzone(sev.motion.rx), deadzone(sev.motion.ry), deadzone(sev.motion.rz));

					drop=true;
				}

				msleep(20);
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

