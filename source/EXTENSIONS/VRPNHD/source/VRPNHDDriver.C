#include <cstdio>
#include <VRPNHDDriver.h>
#include <quat.h>
#include <BALL/MATHS/vector3.h>
#include <BALL/MATHS/quaternion.h>


void tracker_handler(void* userdata, const vrpn_TRACKERCB t)
{
	//for (int i=0; i< a.num_channel; i++)
	//{
	// 	printf("C%d = %+1.4lf   ", i, a.channel[i]);
	//	printf("\n");
	//}
	((BALL::VIEW::VRPNHDDriver *)userdata)->handle_function( t.sensor, t.pos[0], t.pos[1], t.pos[2],
	                                                         t.quat[0], t.quat[1], t.quat[2], t.quat[3]);
	//double matrix[4][4];
	//q_to_row_matrix(matrix, t.quat);
	//printf("%+1.2f  %+1.2f  %+1.2f  %+1.2f\n", t.quat[0], t.quat[1], t.quat[2], t.quat[3]);
	//printf("%+1.1f  %+1.1f  %+1.1f  %+1.1f\n", matrix[0][0], matrix[0][1], matrix[0][2], matrix[0][3]);
	//printf("%+1.1f  %+1.1f  %+1.1f  %+1.1f\n", matrix[1][0], matrix[1][1], matrix[1][2], matrix[1][3]);
	//printf("%+1.1f  %+1.1f  %+1.1f  %+1.1f\n", matrix[2][0], matrix[2][1], matrix[2][2], matrix[2][3]);
	//printf("%+1.1f  %+1.1f  %+1.1f  %+1.1f\n\n", matrix[3][0], matrix[3][1], matrix[3][2], matrix[3][3]);
	//double euler[3];
	//q_to_euler(euler, t.quat);
	//printf("%+1.4f %+1.4f %+1.4f \n", euler[0], euler[1], euler[2]);
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

		void VRPNHDDriver::dz(double& q1, double& q2, double& q3, double& q4) 
		{
			Quaternion q(q1, q2, q3, q4);

			double threshold = 0.2;

			Vector3 axis  = q.getAxis();
			float   angle = q.getAngle();

			// take care of the angle first
			int sign = angle > 0 ? 1 : -1;
			angle = (angle*sign) > threshold ? angle - sign*threshold : 0;
			angle *= 0.05;

			// set small components of the rotation axis to zero
			sign = axis.x > 0 ? 1 : -1;
			axis.x = (axis.x*sign) > threshold ? axis.x - sign*threshold : 0;
			sign = axis.y > 0 ? 1 : -1;
			axis.y = (axis.y*sign) > threshold ? axis.y - sign*threshold : 0;
			sign = axis.z > 0 ? 1 : -1;
			axis.z = (axis.z*sign) > threshold ? axis.z - sign*threshold : 0;

			// do we have an axis left??
			if ((axis.getSquareLength() < 1e-6) || (fabs(angle) < 1e-4))
			{
return;
				q1 = 0;
				q2 = 1;
				q3 = 0;
				q4 = 1e-8;
			}
			else
			{
				q.set(axis, angle);
				q1 = q.i;
				q2 = q.j;
				q3 = q.k;
				q4 = q.angle;
			}
		}
		
		
		void VRPNHDDriver::handle_function(int s, double x, double y, double z, double q1, double q2, double q3, double q4)
		{
			//printf("x=%d, y=%d, z=%d, rx=%d, ry=%d, rz=%d\n", deadzone(x), deadzone(y), deadzone(z), 
			//																								deadzone(rx), deadzone(ry), deadzone(rz));
			if(!vrpn_got_report)
			{
				//if(s==0)
				//{
				//	emitHeadChange(x, z, y, q1, q2, q3, q4 );
				//	printf("HEAD: x=%+1.4f, y=%+1.4f, z=%+1.4f, q1=%+1.4f, q2=%+1.4f, q3=%+1.4f, q4=%+1.4f\n", x, y, z, q1, q2, q3, q4);
				//}
				//else if(s==1)
				//{
				//	//dz(q1, q2, q3, q4);
				//	//emitPositionChange(100*x, z*100, -y*100, dz(q1)*0.1, dz(q2)*0.1, dz(q3)*0.1, dz(q4)*0.1);
					emitPositionChange(100*x, z*100, -y*100, q1, q3, q2, q4);
				//	//printf("MOTION: x=%+1.4f, y=%+1.4f, z=%+1.4f, q1=%+1.4f, q2=%+1.4f, q3=%+1.4f, q4=%+1.4f\n", x, y, z, q1, q2, q3, q4);
				//}
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
					msleep(40);
				}
			}
		}

		bool VRPNHDDriver::setUp()
		{
			if (!tracker_)
			{
				tracker_ = new vrpn_Tracker_Remote((server_.toStdString()).c_str());
				tracker_->register_change_handler(this, tracker_handler);
				//tracker_->set_update_rate(100);
				//tracker_->reset_origin();
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

