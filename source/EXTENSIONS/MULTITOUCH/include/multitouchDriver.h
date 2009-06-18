#ifndef MULTITOUCHDRIVER_H
#define MULTITOUCHDRIVER_H

#include <BALL/VIEW/INPUT/inputDeviceDriver.h>
#include <BALL/SYSTEM/networking.h>

#include <QtCore/QThread>

namespace BALL
{
	namespace VIEW
	{
		class MultitouchDriver 
			: public InputDeviceDriver, 
				protected QThread
		{
			public:
				MultitouchDriver(QWidget* receiver);

				bool setUp();
				bool tearDown();

				void setHostName(const String& hostname) { hostname_ = hostname; }
				void setPort(const String& port) { port_ = port; }
				void setSensitivity(const Size sensitivity)  { printf("sensi: %d\n", sensitivity); sensitivity_ = sensitivity; }

				void setEnabled(bool enabled);

				void receiveValues();
				void sendCamera();
				void focusMolecule();

			private:
				void run();
				float deadzone(float x);

				TCPIOStream connection_;

				String hostname_;
				String port_;
				Size sensitivity_;
		};

	}
}

#endif //MULTITOUCHDRIVER_H

