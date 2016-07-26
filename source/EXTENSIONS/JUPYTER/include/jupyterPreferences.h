// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef JUPYTERPREFERENCES_H
#define JUPYTERPREFERENCES_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#ifndef BALL_VIEW_PLUGIN_VIEWPLUGIN_H
# include <BALL/VIEW/PLUGIN/VIEWPlugin.h>
#endif

#include <ui_jupyterPreferences.h>

#include <QtWidgets/QWidget>

namespace BALL
{
	namespace VIEW
	{

		/** JupyterPreferences is a widget that will be inserted into the
				tab dialog Preferences.
				\ingroup ViewDialogs
		*/
		class JupyterPreferences
			: public ConfigDialog,
			  public Ui_JupyterPreferencesData
		{
			Q_OBJECT

			public:

				enum class ConnectionMode {EXTERNAL, HOSTED};
			
				/// Default Constructor.
				JupyterPreferences(QWidget *parent = nullptr, const char *name = "JupyterPreferences", Qt::WindowFlags fl=0);
				
				/// Destructor.
				virtual ~JupyterPreferences();

				///
				virtual void storeValues();

				///
				void getSettings();

				void setConnectionMode(ConnectionMode mode);

			public slots:
				void setConnectionMode(int index);
				void selectExePath();
				void selectNbdir();

			protected:
				ConnectionMode conn_mode_;
		};
	}
} // namespace

#endif // JUPYTERPREFERENCES_H
