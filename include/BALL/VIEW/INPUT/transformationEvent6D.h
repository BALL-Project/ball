#ifndef TRANSFORMATION6DEVENT_H
#define TRANSFORMATION6DEVENT_H

#include <QtCore/QEvent>


#include <BALL/MATHS/vector3.h>

namespace BALL
{
	namespace VIEW
	{

		class TransformationEvent6D : public QEvent
		{
			public:
				TransformationEvent6D(double x, double y, double z, double a, double b, double c);

				const Vector3& getTranslation() { return translation_; }
				const Vector3& getRotation() { return rotation_; }
			private:
				Vector3 translation_;
				Vector3 rotation_;
		};
	}
}

#endif //TRANSFORMATION6DEVENT_H

