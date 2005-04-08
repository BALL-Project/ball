//   // -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_DIALOGS_DOCKPROGRESSDIALOG_H
#define BALL_VIEW_DIALOGS_DOCKPROGRESSDIALOG_H

#ifndef BALL_VIEW_KERNEL_MODULARWIDGET_H
# include <BALL/VIEW/KERNEL/modularWidget.h>
#endif

#ifndef BALL_VIEW_KERNEL_PREFERENCESENTRY
# include <BALL/VIEW/KERNEL/preferencesEntry.h>
#endif

#ifndef BALL_DATATYPE_OPTIONS_H
# include <BALL/DATATYPE/options.h>
#endif

#include "dockProgressDialogData.h"

namespace BALL
{
	namespace VIEW
	{
		class BALL_EXPORT DockProgressDialog : 
			public DockProgressDialogData,
			public ModularWidget,
			public PreferencesEntry
		{ 
			Q_OBJECT
			BALL_EMBEDDABLE(DockProgressDialog,ModularWidget)
				
			public:
			
				// Constructor
				DockProgressDialog(QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0)
					throw();
					
				// Destructor	
				~DockProgressDialog()
					throw();
			
				//
				void fillDialog(QString p1, QString p2, QString alg, QString sf, Options& alg_opt, Options& sf_opt)
					throw();
					
			public slots:
			
			//
			virtual void pauseClicked();
			
			//
			virtual void abortClicked();
			
			
			protected:
			
			private:
		};
		
} } // Namespaces
#endif
