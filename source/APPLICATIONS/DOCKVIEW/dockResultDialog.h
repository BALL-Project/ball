//   // -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_DIALOGS_DOCKRESULTDIALOG_H
#define BALL_VIEW_DIALOGS_DOCKRESULTDIALOG_H

#ifndef BALL_VIEW_KERNEL_MODULARWIDGET_H
# include <BALL/VIEW/KERNEL/modularWidget.h>
#endif

#ifndef BALL_VIEW_KERNEL_PREFERENCESENTRY
# include <BALL/VIEW/KERNEL/preferencesEntry.h>
#endif

#include "dockResultDialogData.h"

namespace BALL
{
	namespace VIEW
	{
		class BALL_EXPORT DockResultDialog : 
				public DockResultDialogData,
				public ModularWidget,
				public PreferencesEntry
		{ 
				Q_OBJECT
				BALL_EMBEDDABLE(DockResultDialog,ModularWidget)
				
				public:
				
					//Constructor
					DockResultDialog(QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0)
						throw();
						
					//Destructor	
					~DockResultDialog()
						throw();
				
				public slots:
				
				//
					virtual void showSnapshot();
				
				protected:
				
				private:
				
		};
		
} } // Namespaces
#endif
