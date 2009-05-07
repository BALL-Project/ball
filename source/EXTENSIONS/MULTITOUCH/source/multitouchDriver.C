#include <multitouchDriver.h>

namespace BALL
{
	namespace VIEW
	{

		MultitouchDriver::MultitouchDriver(QWidget* receiver)
			: InputDeviceDriver(receiver)
		{
		}

		// TODO
		float MultitouchDriver::deadzone(float x) 
		{
			//int sign = x > 0 ? 1 : -1;
			//return x * sign > 50 ? x - sign * 50 : 0;
			return x;
		}

		void MultitouchDriver::run()
		{
			while(isEnabled() && connection_.good()) 
			{
				// TODO: socket aufmachen
				//       daten lesen
				//       daten an ballview geben
				Size command_index;
				connection_ >> command_index;

				std::cout << command_index << std::endl;
				if (command_index != 1)
					continue;
				
				// TODO scalierungsfaktoren
				// TODO bezugsrahmen Boundingbox-dimensionen

				float x, y, z, rx, ry, rz;

				connection_ >> x;
				connection_ >> y;
				connection_ >> z;
				connection_ >> rx;
				connection_ >> ry;
				connection_ >> rz;	

				std::cout << x << " " << y << " " << z << " " << rx << " " << ry << " " << rz << std::endl;

				emitPositionChange( deadzone(-x), deadzone(-y), deadzone(-z),
														deadzone(rx), deadzone(ry), deadzone(z));

				msleep(35);
			}

			if (!connection_.good())
			{
				std::cout << "Connection to multitouch died!" << std::endl;
				setEnabled(false);
			}
		}

		bool MultitouchDriver::setUp()
		{
			connection_.connect(hostname_, port_);
			connection_ << "Hallo Anne!" << std::endl;
			connection_.flush();

			// TODO: error handling
			return true;
		}

		bool MultitouchDriver::tearDown()
		{
			connection_.close();

			return true;
		}

		void MultitouchDriver::setEnabled(bool enabled)
		{
			InputDeviceDriver::setEnabled(enabled);

			if(enabled) {
				start();
			}
		}

	}
}

