//   // -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_DIALOGS_DOCKINGPROGRESSDIALOG_H
#define BALL_VIEW_DIALOGS_DOCKINGPROGRESSDIALOG_H

#ifndef BALL_VIEW_KERNEL_MODULARWIDGET_H
# include <BALL/VIEW/KERNEL/modularWidget.h>
#endif

#ifndef BALL_VIEW_KERNEL_PREFERENCESENTRY
# include <BALL/VIEW/KERNEL/preferencesEntry.h>
#endif

#include "dockingProgressDialogData.h"

namespace BALL
{
	namespace VIEW
	{
		class BALL_EXPORT DockingProgressDialog : 
				public DockingProgressDialogData,
				public ModularWidget,
				public PreferencesEntry
		{ 
				Q_OBJECT
				BALL_EMBEDDABLE(DockingProgressDialog,ModularWidget)
				
				public:
				
					//Constructor
					DockingProgressDialog(QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0)
						throw();
						
					//Destructor	
					~DockingProgressDialog()
						throw();
				
				public slots:
				
				protected:
				
				private:
				
		};
		
} } // Namespaces
#endif
