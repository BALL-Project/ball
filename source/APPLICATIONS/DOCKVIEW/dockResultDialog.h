//   // -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_DIALOGS_DOCKRESULTDIALOG_H
#define BALL_VIEW_DIALOGS_DOCKRESULTDIALOG_H

#ifndef BALL_VIEW_KERNEL_MODULARWIDGET_H
# include <BALL/VIEW/KERNEL/modularWidget.h>
#endif

#ifndef BALL_VIEW_KERNEL_MAINCONTROL_H
# include <BALL/VIEW/KERNEL/mainControl.h>
#endif

#ifndef BALL_STRUCTURE_DOCKING_CONFORMATIONSET_H
# include <BALL/STRUCTURE/DOCKING/conformationSet.h>
#endif

#include "dockResultDialogData.h"

namespace BALL
{
	namespace VIEW
	{
		class BALL_EXPORT DockResultDialog : 
				public DockResultDialogData,
				public ModularWidget
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
				
					void setConformationSet(ConformationSet conformations)
						throw() {ranked_conformations_ = conformations;}
						
					ConformationSet getConformationSet()
						throw() {return ranked_conformations_;}
					
					// add docked system to BALLView structures 
					void displayDockedSystem()
						throw();
							
				public slots:
				
					/// Show and raise
					void show();
				
					//
					virtual void showSnapshot();
				
				protected:
				
				private:
					ConformationSet ranked_conformations_;
					System* docked_system_;
					
					
		};
		
} } // Namespaces
#endif
