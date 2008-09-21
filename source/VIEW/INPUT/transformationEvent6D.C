#include <BALL/VIEW/INPUT/transformationEvent6D.h>
#include <BALL/VIEW/KERNEL/common.h>

namespace BALL
{
	namespace VIEW
	{

		TransformationEvent6D::TransformationEvent6D(InputDeviceDriver* dev, 
		                                           double x, double y, double z,
	                                             double a, double b, double c)
			: DeviceEvent(TRANSFORMATION_EVENT_6D, dev), translation_(x, y, z), rotation_(a, b, c)
		{
		}

	}
}

