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
				VRPNDriver(QWidget* receiver, QString server_name);

				void handle_function(double x, double y, double z, double rx, double ry, double rz);
				bool setUp();
				bool tearDown();
				int vrpn_got_report;

				void setEnabled(bool enabled);
				

			private:
				void run();

				int deadzone(double x);
				
				vrpn_Analog_Remote* analog;
		};

	}
}

#endif //VRPNDRIVER_H

