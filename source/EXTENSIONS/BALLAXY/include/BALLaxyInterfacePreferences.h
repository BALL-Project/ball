// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALLAXYINTERFACEPREFERENCES_H
#define BALLAXYINTERFACEPREFERENCES_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#ifndef BALL_VIEW_PLUGIN_VIEWPLUGIN_H
# include <BALL/VIEW/PLUGIN/VIEWPlugin.h>
#endif

#include <ui_BALLaxyInterfacePreferences.h>

#include <QtWidgets/QWidget>

namespace BALL
{
	namespace VIEW
	{

		/** BALLaxyInterfacePreferences is a widget that will be inserted into the
				tab dialog Preferences.
				\ingroup ViewDialogs
		*/
		class BALLaxyInterfacePreferences 
			: public ConfigDialog,
			  public Ui_BALLaxyInterfacePreferencesData
		{
			Q_OBJECT

			public:
			
				/// Default Constructor.
				BALLaxyInterfacePreferences(QWidget *parent = NULL, const char *name = "BALLaxyInterfacePreferences", Qt::WindowFlags fl=0);
				
				/// Destructor.
				virtual ~BALLaxyInterfacePreferences();

				///
				virtual void storeValues();

				///
				void getSettings();

				///
				QUrl getBALLaxyBaseUrl();
		};
			
	} 
} // namespace

#endif // BALLAXYINTERFACEPREFERENCES_H
