#ifndef BALL_VIEW_PLUGIN_MODULARWIDGETPLUGINHANDLER_H
#define BALL_VIEW_PLUGIN_MODULARWIDGETPLUGINHANDLER_H

#ifndef BALL_PLUGIN_PLUGINHANDLER_H
# include <BALL/PLUGIN/pluginHandler.h>
#endif

#ifndef BALL_VIEW_KERNEL_MODULARWIDGET_H
# include <BALL/VIEW/KERNEL/modularWidget.h>
#endif

namespace BALL
{
	class BALLPlugin;

	namespace VIEW
	{
		class MainControl;

		/**
		 * See PluginHandler for detailed documentation
		 */
		class BALL_VIEW_EXPORT ModularWidgetPluginHandler 
			: public PluginHandler
		{
			public:
				ModularWidgetPluginHandler(MainControl* parent);

				virtual bool canHandle(BALLPlugin* plugin) const;

			protected:
				virtual bool specificSetup_(BALLPlugin* plugin);
				virtual bool specificShutdown_(BALLPlugin* plugin);

				MainControl*   main_control_;
				ModularWidget* modular_widget_;
		};
	}
}

#endif // BALL_VIEW_PLUGIN_MODULARWIDGETPLUGINHANDLER_H
