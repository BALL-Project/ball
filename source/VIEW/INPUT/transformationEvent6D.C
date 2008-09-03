#include <BALL/VIEW/INPUT/transformationEvent6D.h>

namespace BALL
{
	namespace VIEW
	{

		TransformationEvent6D::TransformationEvent6D(double x, double y, double z,
	                                             double a, double b, double c)
			: QEvent(QEvent::User), translation_(x, y, z), rotation_(a, b, c)
		{
		}

	}
}

