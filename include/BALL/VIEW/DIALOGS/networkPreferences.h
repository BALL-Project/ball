// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_DIALOGS_NETWORKPREFERENCES_H
#define BALL_VIEW_DIALOGS_NETWORKPREFERENCES_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#ifndef BALL_VIEW_KERNEL_PREFERENCESENTRY
# include <BALL/VIEW/KERNEL/preferencesEntry.h>
#endif

#include <BALL/VIEW/UIC/ui_networkPreferences.h>

#include <QtWidgets/QWidget>

namespace BALL
{
	namespace VIEW
	{

		/** NeworkPreferences is a widget that will be inserted into the
				tab dialog Preferences.
				\ingroup ViewDialogs
		*/
		class BALL_VIEW_EXPORT NetworkPreferences 
			: public QWidget,
				public Ui_NetworkPreferencesData,
				public PreferencesEntry
		{
			Q_OBJECT

			public:
			
				/// Default Constructor.
				NetworkPreferences(QWidget *parent = NULL, const char *name = "NetworkPreferences", Qt::WindowFlags fl=0);
				
				/// Destructor.
				virtual ~NetworkPreferences();

				///
				void applySettings();

				///
				void getSettings();

				///
				bool proxyEnabled();

				public slots:

				///
				void proxyModeChanged(int proxy_mode);

		};
			
	} 
} // namespace

#endif // BALL_VIEW_DIALOGS_NETWORKPREFERENCES_H
