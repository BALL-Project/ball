
#ifndef BALL_EXTENSIONS_MULTITOUCHSETTINGS_H
#define BALL_EXTENSIONS_MULTITOUCHSETTINGS_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#ifdef BALL_COMPILER_MSVC
# include "../include/ui_multitouchSettings.h"
#else
# include "../include/multitouchSettingsData.h"
#endif

// TODO: register as a preferencesentry
#ifndef BALL_VIEW_KERNEL_PREFERENCESENTRY
# include <BALL/VIEW/KERNEL/preferencesEntry.h>
#endif

#include "multitouchPlugin.h"
#include <QtGui/QDialog>

namespace BALL
{
	namespace VIEW
	{
		class MultitouchSettings 
			: public QDialog,
				public Ui_multitouchSettingsData,
				public PreferencesEntry
		{ 
			Q_OBJECT

			public:

				/// Constructor
				MultitouchSettings(MultitouchPlugin* plugin, QWidget* parent = 0, 
													 const char* name = "MultitouchSettings", Qt::WFlags fl = 0);

				/// Destructor
				~MultitouchSettings() {}

				void setHostName(const String& hostname);
				void setPort(const String& port);
				void setSensitivity(const Size sensitivity);

			public slots:
				virtual void accept();

			protected:
				MultitouchPlugin* plugin_;
		};

	}
}

#endif
