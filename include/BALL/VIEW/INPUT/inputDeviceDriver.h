#ifndef INPUTDEVICEDRIVER_H
#define INPUTDEVICEDRIVER_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#include <QtCore/QMutex>

class QWidget;

namespace BALL
{
	namespace VIEW
	{
		class Scene;

		class BALL_VIEW_EXPORT InputDeviceDriver
		{
			public:
				InputDeviceDriver(QWidget* reciever);
				virtual ~InputDeviceDriver();

				virtual bool setUp() = 0;
				virtual bool tearDown() = 0;

				void setEnabled(bool enabled);
				bool isEnabled() { return enabled_; }

				void setReceiver(Scene* s);
				QWidget* getReceiver();

			protected:
				void emitPositionChange(double x, double y, double z, double a, double b, double c);
				void emitPositionChange(double x, double y, double z, double q1, double q2, double q3, double q4);
				void emitHeadChange(double x, double y, double z, double q1, double q2, double q3, double q4);

			private:
				QMutex receiverLock;

				QWidget* receiver_;
				bool enabled_;
		};

	}
}

#endif //INPUTDEVICEDRIVER_H

