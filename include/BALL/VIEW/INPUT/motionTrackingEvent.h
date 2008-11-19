#ifndef MOTIONTRACKINGEVENT_H
#define MOTIONTRACKINGEVENT_H

#include <QtCore/QEvent>

#include <BALL/VIEW/INPUT/deviceEvent.h>
#include <BALL/MATHS/vector3.h>
#include <BALL/MATHS/quaternion.h>

namespace BALL
{
	namespace VIEW
	{
		class MotionTrackingEvent : public DeviceEvent
		{
			public:
				MotionTrackingEvent(InputDeviceDriver* dev, double x, double y, double z, 
				                                            double q1, double q2, double q3, double q4);

				const Vector3& getOrigin() { return origin_; }
				const Quaternion& getTransform() { return transform_; }
			private:
				Vector3 origin_;
				Quaternion transform_;
		};
	}
}

#endif //MOTIONTRACKINGEVENT_H

