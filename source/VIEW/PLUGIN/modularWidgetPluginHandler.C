#include <BALL/VIEW/PLUGIN/modularWidgetPluginHandler.h>

#include <BALL/PLUGIN/BALLPlugin.h>
#include <BALL/PLUGIN/pluginManager.h>

#include <BALL/VIEW/PLUGIN/modularWidgetPlugin.h>

#include <BALL/VIEW/KERNEL/mainControl.h>

namespace BALL
{
	namespace VIEW
	{
		ModularWidgetPluginHandler::ModularWidgetPluginHandler(MainControl* parent)
		{
			main_control_ = parent;
		}

		bool ModularWidgetPluginHandler::canHandle(BALLPlugin* plugin) const
		{
			return qobject_cast<ModularWidgetPlugin*>(plugin) != 0;
		}

		bool ModularWidgetPluginHandler::specificSetup_(BALLPlugin* plugin)
		{
			qobject_cast<ModularWidgetPlugin*>(plugin)->setMainControl(main_control_);
			return plugin->activate();
		}

		bool ModularWidgetPluginHandler::specificShutdown_(BALLPlugin* plugin)
		{
			return plugin->deactivate();
		}
	}
}
