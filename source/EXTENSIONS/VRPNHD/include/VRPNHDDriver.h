#ifndef VRPNHDDRIVER_H
#define VRPNHDDRIVER_H

#include <BALL/VIEW/INPUT/inputDeviceDriver.h>
#include <vrpn_Tracker.h>
#include <QtCore/QThread>

namespace BALL
{
	namespace VIEW
	{

		class VRPNHDDriver : public InputDeviceDriver, protected QThread
		{
			public:
				VRPNHDDriver();
				VRPNHDDriver(QWidget* receiver);

				bool setUp();
				bool tearDown();
				
				void handle_function(int s, double x, double y, double z, double q1, double q2, double q3, double q4);

				void setEnabled(bool enabled);
				void setServer(QString server);
				
				int vrpn_got_report;
			private:
				void run();
				void dz(double& q1, double& q2, double& q3, double& q4);
				
				QString server_;
				vrpn_Tracker_Remote* tracker_;
		};

	}
}

#endif //VRPNHDDRIVER_H

