#ifndef BALL_EXTENSIONS_XML3DSETTINGS_H
#define BALL_EXTENSIONS_XML3DSETTINGS_H

#include "XML3DPlugin.h"

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#include "ui_XML3DSettings.h"

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
		class XML3DSettings 
			: public ConfigDialog,
			  public Ui_XML3DSettingsData
		{ 
			Q_OBJECT

			public:

				/// Constructor
				XML3DSettings(XML3DPlugin* plugin, QWidget* parent = 0, 
													 const char* name = "XML3DSettings", Qt::WindowFlags fl = 0);

				/// Destructor
				~XML3DSettings() {}

				void setHostName(const String& hostname);
				void setPort(const String& port);
				// TODO: remove!
				void setSensitivity(const Size sensitivity);

			public slots:
				virtual void accept();

			protected:
				XML3DPlugin* plugin_;
		};

	}
}

#endif
