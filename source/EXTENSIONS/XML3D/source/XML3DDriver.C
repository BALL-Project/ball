
#include <XML3DDriver.h>

#include <BALL/STRUCTURE/geometricProperties.h>
#include <BALL/VIEW/WIDGETS/scene.h>
#include <BALL/VIEW/KERNEL/compositeManager.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/COMMON/limits.h>

namespace BALL
{
	namespace VIEW
	{

		XML3DDriver::XML3DDriver(QWidget* receiver)
			: InputDeviceDriver(receiver),
				TCPServerThread(4711, true, true),
				sensitivity_(20)
		{
		}


		void XML3DDriver::handleAsyncConnection()
		{
			Scene* scene = dynamic_cast<Scene*>(getReceiver());

			if (!scene)
			{
				String result = "Error: receiver for XML3D events is not a scene!";
				connected_stream_ << result.length() << " " << result << std::endl;
			}
			else
			{
				std::ostringstream result;

				scene->dumpXML3D(result);
				connected_stream_ << result.str().length() << " " << result.str() << std::endl;
			}
		}

		bool XML3DDriver::setUp()
		{
			// TODO: error handling
			return true;
		}

		bool XML3DDriver::tearDown()
		{
			deactivate();
			wait(); // need the wait here?
			return true;
		}

		void XML3DDriver::setEnabled(bool enabled)
		{
			InputDeviceDriver::setEnabled(enabled);

			if(enabled) 
			{
				start();
			}
		}

	}
}

