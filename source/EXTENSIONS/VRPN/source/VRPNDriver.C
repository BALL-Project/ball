#include <cstdio>
#include <VRPNDriver.h>


void analog_handler(void* userdata, const vrpn_ANALOGCB a)
{
	//for (int i=0; i< a.num_channel; i++)
	//{
	// 	printf("C%d = %+1.4lf   ", i, a.channel[i]);
	//	printf("\n");
	//}
	((BALL::VIEW::VRPNDriver *)userdata)->handle_function( a.channel[0], a.channel[1], a.channel[2],
	                                                       a.channel[3], a.channel[5], a.channel[4]);
}

namespace BALL
{
	namespace VIEW
	{

		
		VRPNDriver::VRPNDriver(QWidget* receiver)
			: InputDeviceDriver(receiver),
				analog_(0)
		{
		}

		int VRPNDriver::deadzone(double x) {
			x*=1000;
			int sign = x > 0 ? 1 : -1;
			return x * sign > 50 ? x - sign * 50 : 0;
		}
		
		
		void VRPNDriver::handle_function(double x, double y, double z, double rx, double ry, double rz)
		{
			//printf("x=%+1.4f, y=%+1.4f, z=%+1.4f, rx=%+1.4f, ry=%+1.4f, rz=%+1.4f\n", x, y, z, rx, ry, rz);
			//printf("x=%d, y=%d, z=%d, rx=%d, ry=%d, rz=%d\n", deadzone(x), deadzone(y), deadzone(z), 
			//																								deadzone(rx), deadzone(ry), deadzone(rz));
			if(!vrpn_got_report)
			{
			emitPositionChange( deadzone(-x), deadzone(z), deadzone(y),
			                    deadzone(rx), deadzone(-ry), deadzone(-rz) );
			}
			vrpn_got_report = 1;
		}
		

		void VRPNDriver::run()
		{

			while(isEnabled())
			{
				analog_->mainloop();
				vrpn_got_report = 0;
				while (!vrpn_got_report)
				{
					analog_->mainloop();
					msleep(35);
				}
			}
		}

		bool VRPNDriver::setUp()
		{
			if (!analog_)
			{
				analog_ = new vrpn_Analog_Remote((server_.toStdString()).c_str());
				analog_->register_change_handler(this, analog_handler);
				return true;
			}
			else {return false;}
		}

		bool VRPNDriver::tearDown()
		{
			delete analog_;
			return true;
		}

		void VRPNDriver::setEnabled(bool enabled)
		{
			InputDeviceDriver::setEnabled(enabled);

			if(enabled)
			{
				start();
			}
		}

		void VRPNDriver::setServer(QString server)
		{
			server_ = server; 
		}
	}
}

