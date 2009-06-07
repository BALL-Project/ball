#include <multitouchDriver.h>

#include <BALL/STRUCTURE/geometricProperties.h>
#include <BALL/VIEW/WIDGETS/scene.h>
#include <BALL/VIEW/KERNEL/compositeManager.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/COMMON/limits.h>

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

		void MultitouchDriver::receiveValues()
		{
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
			emitPositionChange( 10.*deadzone(-x), 10.*deadzone(-y), 10.*deadzone(-z),
							 					1./3.*deadzone(rx), 1./3.*deadzone(ry), 1./3.*deadzone(rz));
		}

		void MultitouchDriver::sendCamera()
		{
			Scene* scene = dynamic_cast<Scene*>(getReceiver());

			if (!scene)
			{
				Log.error() << "Error: receiver for multitouch events is not a scene!" << std::endl;	
				return;
			}
	
			Stage* stage = scene->getStage();

			Vector3 const& viewpoint = stage->getCamera().getViewPoint();
			connection_ << viewpoint.x << " " << viewpoint.y << " " << viewpoint.z << " ";

			CompositeManager const& cm = scene->getMainControl()->getCompositeManager();

			Vector3 lower(Limits<float>::max()), upper(Limits<float>::min());
			BoundingBoxProcessor bp;

			for (CompositeManager::CompositeConstIterator ci = cm.begin(); +ci; ++ci)
			{
				(*ci)->apply(bp);
				Vector3 const& b_lower = bp.getLower();
				Vector3 const& b_upper = bp.getUpper();

				lower.x = std::min(b_lower.x, lower.x);
				lower.y = std::min(b_lower.y, lower.y);
				lower.z = std::min(b_lower.z, lower.z);

				upper.x = std::max(b_upper.x, upper.x);
				upper.y = std::max(b_upper.y, upper.y);
				upper.z = std::max(b_upper.z, upper.z);
			}

			connection_ << lower.x << " " << lower.y << " " << lower.z << " "; 
			connection_ << upper.x << " " << upper.y << " " << upper.z << std::endl << std::flush;
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
				switch (command_index)
				{
					case 1:	receiveValues();
									break;
					case 2: sendCamera();
									break;
				}
				msleep(35);
			}

			if (!connection_.good())
			{
				std::cout << "Connection to Multi Touch died!" << std::endl;
				setEnabled(false);
			}
		}

		bool MultitouchDriver::setUp()
		{
			connection_.connect(hostname_, port_);
			connection_.flush();

			// TODO: error handling
			return true;
		}

		bool MultitouchDriver::tearDown()
		{
			connection_ << " Bye! MultitouchDriver::tearDown()" << std::endl;
			connection_.close();

			return true;
		}

		void MultitouchDriver::setEnabled(bool enabled)
		{
			InputDeviceDriver::setEnabled(enabled);

			if(enabled) 
			{
				start();
			}
		}

	}
}

