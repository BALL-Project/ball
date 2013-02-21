#include <BALL/VIEW/PLUGIN/inputDevPluginHandler.h>

#include <BALL/PLUGIN/BALLPlugin.h>

#include <BALL/VIEW/INPUT/inputPlugin.h>
#include <BALL/VIEW/WIDGETS/scene.h>

namespace BALL
{
	namespace VIEW
	{
		bool InputDevPluginHandler::canHandle(BALLPlugin* plugin) const
		{
			return qobject_cast<InputPlugin*>(plugin) != 0;
		}

		bool InputDevPluginHandler::specificSetup_(BALLPlugin* plugin)
		{
			InputPlugin* ptr = qobject_cast<InputPlugin*>(plugin);
			ptr->setReceiver(Scene::getInstance(0));

			return plugin->activate();
		}

		bool InputDevPluginHandler::specificShutdown_(BALLPlugin* plugin)
		{
			return plugin->deactivate();
		}
	}
}

