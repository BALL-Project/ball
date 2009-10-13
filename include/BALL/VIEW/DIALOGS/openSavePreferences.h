#ifndef BALL_VIEW_DIALOGS_OPENSAVEPREFERENCES_H
#define BALL_VIEW_DIALOGS_OPENSAVEPREFERENCES_H

#ifndef BALL_VIEW_KERNEL_PREFERENCESENTRY
# include <BALL/VIEW/KERNEL/preferencesEntry.h>
#endif

#include <BALL/VIEW/UIC/ui_openSavePreferences.h>

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
				bool binaryProjectsEnabled();
		};
	}
}

#endif //BALL_VIEW_DIALOGS_OPENSAVEPREFERENCES_H
