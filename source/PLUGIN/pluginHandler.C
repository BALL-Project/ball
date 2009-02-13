#include <BALL/PLUGIN/pluginHandler.h>
#include <BALL/PLUGIN/BALLPlugin.h>

namespace BALL
{

	bool PluginHandler::isRunning(BALLPlugin* plugin) const
	{
		std::list<BALLPlugin*>::const_iterator it = running_plugins_.begin();
		for(; it != running_plugins_.end(); ++it) {
			if(*it == plugin) {
				return true;
			}
		}

		return false;
	}

	bool PluginHandler::startPlugin(BALLPlugin* plugin)
	{
		if(plugin && canHandle(plugin) && !isRunning(plugin) && specificSetup_(plugin)) {
			running_plugins_.push_back(plugin);
			return true;
		}

		return false;
	}

	bool PluginHandler::stopPlugin(BALLPlugin* plugin)
	{
		if(plugin && isRunning(plugin) && specificShutdown_(plugin)) {
			running_plugins_.remove(plugin);
			return true;
		}

		return false;
	}

}

