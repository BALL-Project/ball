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

		ModularWidgetPluginHandler::~ModularWidgetPluginHandler()
		{
			PluginManager::instance().unregisterHandler(this);
		}

		bool ModularWidgetPluginHandler::canHandle(BALLPlugin* plugin) const
		{
			return qobject_cast<ModularWidgetPlugin*>(plugin) != 0;
		}

		bool ModularWidgetPluginHandler::specificSetup_(BALLPlugin* plugin)
		{
			ModularWidgetPlugin* ptr = qobject_cast<ModularWidgetPlugin*>(plugin);

			// let the plugin create its widget
			modular_widget_ = ptr->createModularWidget(main_control_);

			modular_widget_->registerWidget(modular_widget_);

			// and initialize it
			modular_widget_->initializeWidget(*main_control_);

			return plugin->activate();
		}

		bool ModularWidgetPluginHandler::specificShutdown_(BALLPlugin* plugin)
		{
			// finalize the widget
			modular_widget_->finalizeWidget(*main_control_);
			delete(modular_widget_);

			return plugin->deactivate();
		}
	}
}
