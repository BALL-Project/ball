// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef IPYTHONPREFERENCES_H
#define IPYTHONPREFERENCES_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#ifndef BALL_VIEW_PLUGIN_VIEWPLUGIN_H
# include <BALL/VIEW/PLUGIN/VIEWPlugin.h>
#endif

#include <ui_iPythonPreferences.h>

#include <QtGui/QWidget>

namespace BALL
{
	namespace VIEW
	{

		/** IPythonPreferences is a widget that will be inserted into the
				tab dialog Preferences.
				\ingroup ViewDialogs
		*/
		class IPythonPreferences 
			:	public ConfigDialog,
			  public Ui_IPythonPreferencesData
		{
			Q_OBJECT

			public:
			
				/// Default Constructor.
				IPythonPreferences(QWidget *parent = NULL, const char *name = "IPythonPreferences", Qt::WFlags fl=0);
				
				/// Destructor.
				virtual ~IPythonPreferences();

				///
				virtual void storeValues();

				///
				void getSettings();

				///
				QUrl getIPythonURL();

		};
			
	} 
} // namespace

#endif // IPYTHONPREFERENCES_H
