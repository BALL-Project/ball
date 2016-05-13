#ifndef BALL_EXTENSIONS_PRESENTABALLSETTINGS_H
#define BALL_EXTENSIONS_PRESENTABALLSETTINGS_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#include "ui_PresentaBALLSettings.h"

// TODO: register as a preferencesentry
#ifndef BALL_VIEW_KERNEL_PREFERENCESENTRY
# include <BALL/VIEW/KERNEL/preferencesEntry.h>
#endif

#include <BALL/VIEW/PLUGIN/VIEWPlugin.h>

#include <QtWidgets/QDialog>

namespace BALL
{
	namespace VIEW
	{
		class PresentaBALLView;

		class PresentaBALLSettings
			: public ConfigDialog,
			  public Ui_PresentaBALLSettingsData
		{
			Q_OBJECT

			public:

				/// Constructor
				PresentaBALLSettings(PresentaBALLView* parent = 0,
					const char* name = "PresentaBALLSettings", Qt::WindowFlags fl = 0);

				/// Destructor
				~PresentaBALLSettings();

				virtual void restoreDefaultValues(bool all = false);

				void setIndexHTMLLocation(const QString& path);
				QString getIndexHTMLLocation();

			public slots:
				virtual void selectIndexHTMLLocation();

			protected:
				PresentaBALLView* html_interface_;
		};

	}
}

#endif
