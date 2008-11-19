#include <stdio.h>
#include <VRPNHDDriver.h>


void tracker_handler(void* userdata, const vrpn_TRACKERCB t)
{
	//for (int i=0; i< a.num_channel; i++)
	//{
	// 	printf("C%d = %+1.4lf   ", i, a.channel[i]);
	//	printf("\n");
	//}
	((BALL::VIEW::VRPNHDDriver *)userdata)->handle_function( t.sensor, t.pos[0], t.pos[1], t.pos[2],
	                                                         t.quat[0], t.quat[1], t.quat[2], t.quat[3]);
}

namespace BALL
{
	namespace VIEW
	{

		
		VRPNHDDriver::VRPNHDDriver(QWidget* receiver)
			: InputDeviceDriver(receiver),
				tracker_(0)
		{
		}

		int VRPNHDDriver::deadzone(double x) {
			x*=1000;
			int sign = x > 0 ? 1 : -1;
			return x * sign > 50 ? x - sign * 50 : 0;
		}
		
		
		void VRPNHDDriver::handle_function(int s, double x, double y, double z, double q1, double q2, double q3, double q4)
		{
			printf("s=%d, x=%+1.4f, y=%+1.4f, z=%+1.4f, q1=%+1.4f, q2=%+1.4f, q3=%+1.4f, q4=%+1.4f\n", s, x, y, z, q1, q2, q3, q4);
			//printf("x=%d, y=%d, z=%d, rx=%d, ry=%d, rz=%d\n", deadzone(x), deadzone(y), deadzone(z), 
			//																								deadzone(rx), deadzone(ry), deadzone(rz));
			if(!vrpn_got_report)
			{
				if(s==0)
				{
					emitPositionChange(x, z, y, q1, q2, q3, q4 );
				}
				else if(s==1)
				{
					emitHeadChange(x, z, y, q1, q2, q3, q4);
				}
			}
			vrpn_got_report = 1;
		}
		

		void VRPNHDDriver::run()
		{

			while(isEnabled())
			{
				tracker_->mainloop();
				vrpn_got_report = 0;
				while (!vrpn_got_report)
				{
					tracker_->mainloop();
					msleep(25);
				}
			}
		}

		bool VRPNHDDriver::setUp()
		{
			if (!tracker_)
			{
				tracker_ = new vrpn_Tracker_Remote((server_.toStdString()).c_str());
				tracker_->register_change_handler(this, tracker_handler);
				return true;
			}
			else {return false;}
		}

		bool VRPNHDDriver::tearDown()
		{
			delete tracker_;
			return true;
		}

		void VRPNHDDriver::setEnabled(bool enabled)
		{
			InputDeviceDriver::setEnabled(enabled);

			if(enabled)
			{
				start();
			}
		}

		void VRPNHDDriver::setServer(QString server)
		{
			server_ = server; 
		}
	}
}

