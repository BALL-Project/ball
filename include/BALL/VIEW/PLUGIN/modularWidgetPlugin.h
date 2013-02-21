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

				virtual ModularWidget* createModularWidget(MainControl* parent) = 0;

			protected:
				MainControl* main_control_;
		};
	}
}

Q_DECLARE_INTERFACE(BALL::VIEW::ModularWidgetPlugin, "org.ball-project.Plugin.ModularWidgetPlugin/1.0")

#endif // BALL_VIEW_PLUGIN_MODULARWIDGETPLUGIN_H
