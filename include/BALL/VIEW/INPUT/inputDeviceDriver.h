#ifndef INPUTDEVICEDRIVER_H
#define INPUTDEVICEDRIVER_H

#include <QtCore/QMutex>

namespace BALL
{
	namespace VIEW
	{
		class Scene;

		class InputDeviceDriver
		{
			public:
				//TODO Finalize the constructor, take an associated object
				InputDeviceDriver(Scene* reciever);
				virtual ~InputDeviceDriver();

				virtual bool setUp() = 0;
				virtual bool tearDown() = 0;

				void setEnabled(bool enabled);
				bool isEnabled() { return enabled_; }

				void setReceiver(Scene* s);
				Scene* getReceiver();

			protected:
				void emitPositionChange(double x, double y, double y, double a, double b, double c);

			private:
				QMutex receiverLock;

				Scene* receiver_;
				bool enabled_;
		};

	}
}

#endif //INPUTDEVICEDRIVER_H

