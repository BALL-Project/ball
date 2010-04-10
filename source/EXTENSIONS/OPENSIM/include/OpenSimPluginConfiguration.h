// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_EXTENSIONS_OPENSIM_OPENSIMPLUGINCONFIGURATION_H
#define BALL_EXTENSIONS_OPENSIM_OPENSIMPLUGINCONFIGURATION_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#ifndef BALL_VIEW_KERNEL_PREFERENCESENTRY_H
# include <BALL/VIEW/KERNEL/preferencesEntry.h>
#endif

#include "ui_OpenSimPluginConfiguration.h"

namespace BALL
{
	namespace VIEW
	{

		/** A simple configuration dialog for the OpenSim plugin.
		 */
		class OpenSimPluginConfiguration
			: public QDialog,
				public Ui_OpenSimPluginConfigurationData,
				public PreferencesEntry
		{
			Q_OBJECT

			public:

				/// Constructor
				OpenSimPluginConfiguration(QWidget *parent = NULL, 
				                           const char *name = "OpenSimPluginConfiguration", Qt::WFlags fl = 0);

				/// Destructor
				virtual ~OpenSimPluginConfiguration();

				virtual void readPreferenceEntries(const INIFile& ini);

				void setRemoteServer(const String& address, Size port);	
				void setLocalServer(const String& address,  Size port);	
		};

	}
}

#endif
