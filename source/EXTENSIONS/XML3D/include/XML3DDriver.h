#ifndef XML3DDRIVER_H
#define XML3DDRIVER_H

#include <BALL/VIEW/INPUT/inputDeviceDriver.h>
#include <BALL/SYSTEM/networking.h>

#include <QtCore/QThread>

namespace BALL
{
	namespace VIEW
	{
		class XML3DDriver 
			: public InputDeviceDriver, 
				protected TCPServerThread
		{
			public:
				XML3DDriver(QWidget* receiver);

				bool setUp();
				bool tearDown();

				void setHostName(const String& hostname) { hostname_ = hostname; }
				void setPort(const String& port) { port_ = port; }
				// TODO
				void setSensitivity(const Size sensitivity)  {  sensitivity_ = sensitivity; }

				void setEnabled(bool enabled);

				void handleAsyncConnection();

			private:
				String hostname_;
				String port_;
				Size sensitivity_;
		};

	}
}

#endif //XML3DDRIVER_H

