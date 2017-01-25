// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_PLUGIN_MODULARWIDGETPLUGIN_H
#define BALL_VIEW_PLUGIN_MODULARWIDGETPLUGIN_H

#ifndef BALL_VIEW_PLUGIN_VIEWPLUGIN_H
# include <BALL/VIEW/PLUGIN/VIEWPlugin.h>
#endif

#ifndef BALL_VIEW_KERNEL_MODULARWIDGET_H
# include <BALL/VIEW/KERNEL/modularWidget.h>
#endif

namespace BALL
{
	namespace VIEW
	{
		class MainControl;

		class ModularWidgetPlugin
		{
			public:
				virtual ~ModularWidgetPlugin() {}

				/** Set the main control.
				 *  Each widget plugin has to know which main control is responsible for
				 *  the widget it creates.
				 *
				 *  Note: The main control has to be set before activating the plugin!
				 *
				 *  @param main_control Main control repsonsible for the widget
				 */
				virtual void setMainControl(MainControl* main_control) { main_control_ = main_control; }

			protected:
				MainControl* main_control_;
		};
	}
}

Q_DECLARE_INTERFACE(BALL::VIEW::ModularWidgetPlugin, "org.ball-project.Plugin.ModularWidgetPlugin/1.0")

#endif // BALL_VIEW_PLUGIN_MODULARWIDGETPLUGIN_H
