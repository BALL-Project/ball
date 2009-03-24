#ifndef BALL_VIEW_DIALOGS_OPENSAVEPREFERENCES_H
#define BALL_VIEW_DIALOGS_OPENSAVEPREFERENCES_H

#ifdef BALL_COMPILER_MSVC
# include <BALL/VIEW/UIC/ui_openSavePreferencesData.h>
#else
# include <BALL/VIEW/UIC/openSavePreferencesData.h>
#endif

#ifndef BALL_VIEW_KERNEL_PREFERENCESENTRY
# include <BALL/VIEW/KERNEL/preferencesEntry.h>
#endif

#include <QtGui/QWidget>

namespace BALL
{
	namespace VIEW
	{
		class OpenSavePreferences
			: public QWidget,
			  public Ui_OpenSavePreferencesData,
			  public PreferencesEntry
		{
			Q_OBJECT

			public:
				OpenSavePreferences(QWidget* parent = 0,
				                    const char* name = "Open/Save Preferences",
				                    Qt::WFlags fl = 0);

				~OpenSavePreferences();

				void storeValues();
		};
	}
}

#endif //BALL_VIEW_DIALOGS_OPENSAVEPREFERENCES_H
