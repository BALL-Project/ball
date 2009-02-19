#include <BALL/PLUGIN/pluginHandler.h>
#include <BALL/PLUGIN/BALLPlugin.h>

#include <QtCore/QReadLocker>

namespace BALL
{

	PluginHandler::~PluginHandler()
	{
	}

	bool PluginHandler::isRunning(BALLPlugin* plugin) const
	{
		QReadLocker locker(&mutex_);
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
			mutex_.lockForWrite();
			running_plugins_.push_back(plugin);
			mutex_.unlock();
			return true;
		}

		return false;
	}

	bool PluginHandler::stopPlugin(BALLPlugin* plugin)
	{
		if(plugin && isRunning(plugin) && specificShutdown_(plugin)) {
			mutex_.lockForWrite();
			running_plugins_.remove(plugin);
			mutex_.unlock();
			return true;
		}

		return false;
	}

}

