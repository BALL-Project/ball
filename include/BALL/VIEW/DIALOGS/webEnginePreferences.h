#ifndef BALL_VIEW_DIALOGS_WEBENGINEPREFERENCES_H
#define BALL_VIEW_DIALOGS_WEBENGINEPREFERENCES_H

#include <BALL/VIEW/KERNEL/preferencesEntry.h>
#include <BALL/VIEW/KERNEL/modularWidget.h>
#include <BALL/VIEW/UIC/ui_webEnginePreferences.h>

#include <QtWidgets/QDialog>

namespace BALL
{
	namespace VIEW
	{
		class BALL_VIEW_EXPORT WebEnginePreferences
			: public QDialog,
			  public Ui_WebEnginePreferences,
			  public PreferencesEntry,
			  public ModularWidget
		{
			Q_OBJECT

			public:

				BALL_EMBEDDABLE(WebEnginePreferences, ModularWidget)

				/// Constructor
				WebEnginePreferences(QWidget* parent = 0, const char* name = "WebEngine Preferences", Qt::WindowFlags fl = 0);

				/// Destructor
				~WebEnginePreferences();

				void applySettings();

			public slots:

				void selectScriptBase();

			protected:
				String script_base_;
		};
	}
}

#endif // BALL_VIEW_DIALOGS_WEBENGINEPREFERENCES_H
