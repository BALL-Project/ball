#ifndef VRPNDRIVER_H
#define VRPNDRIVER_H

#include <BALL/VIEW/INPUT/inputDeviceDriver.h>
#include <vrpn_Analog.h>
#include <QtCore/QThread>

namespace BALL
{
	namespace VIEW
	{

		class VRPNDriver : public InputDeviceDriver, protected QThread
		{
			public:
				VRPNDriver();
				VRPNDriver(QWidget* receiver);

				bool setUp();
				bool tearDown();
				
				void handle_function(double x, double y, double z, double rx, double ry, double rz);

				void setEnabled(bool enabled);
				void setServer(QString server);
				
				int vrpn_got_report;

			private:
				void run();
				int deadzone(double x);
				
				QString server_;
				vrpn_Analog_Remote* analog_;
		};

	}
}

#endif //VRPNDRIVER_H

