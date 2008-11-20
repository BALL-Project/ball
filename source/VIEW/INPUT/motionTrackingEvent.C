#include <BALL/VIEW/INPUT/motionTrackingEvent.h>
#include <BALL/VIEW/KERNEL/common.h>

namespace BALL
{
	namespace VIEW
	{

		MotionTrackingEvent::MotionTrackingEvent(InputDeviceDriver* dev, 
		                                           double x, double y, double z,
	                                             	   double q1, double q2, double q3, double q4)
			: DeviceEvent(MOTIONTRACKING_EVENT, dev), origin_(-x, -y, z), transform_(q1, q2, q3, q4)
		{
		}

	}
}

