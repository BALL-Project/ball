//   // -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_DIALOGS_DOCKDIALOG_H
#define BALL_VIEW_DIALOGS_DOCKDIALOG_H

#ifndef BALL_VIEW_KERNEL_MODULARWIDGET_H
# include <BALL/VIEW/KERNEL/modularWidget.h>
#endif

#ifndef BALL_CONCEPT_COMPOSITE_H
# include <BALL/CONCEPT/composite.h>
#endif

#ifndef BALL_VIEW_KERNEL_MAINCONTROL_H
# include <BALL/VIEW/KERNEL/mainControl.h>
#endif

#ifndef BALL_VIEW_KERNEL_MESSAGE_H
# include <BALL/VIEW/KERNEL/message.h>
#endif

#ifndef BALL_VIEW_KERNEL_PREFERENCESENTRY
# include <BALL/VIEW/KERNEL/preferencesEntry.h>
#endif

#include "dockDialogData.h"

namespace BALL
{
	namespace VIEW
	{
		class BALL_EXPORT DockDialog : 
				public DockDialogData,
				public ModularWidget,
				public PreferencesEntry
		{ 
				Q_OBJECT

				public:
				
				/// Constructor
				DockDialog(QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0)
					throw();

				/// Destructor
				virtual ~DockDialog()
					throw();
				
				/**	Initializes the popup menu <b>  Display </b> with its checkable submenu <b>  Docking </b>; 
						This method is called automatically	immediately before the main application is started. 
						@param main_control the  MainControl object to be initialized 
						@see   openDialog
						@see   finalizeWidget
						@see   insertMenuEntry
				*/
				virtual void initializeWidget(MainControl& main_control)
					throw();

				/**	Removes the checkable submenu <b>  Docking </b> from the popup menu <b>  Display </b>.
						This method will be called by  MainControl::aboutToExit.
						@param main_control the  MainControl to be finalized 
						@see   initializeWidget
						@see   removeMenuEntry
						@see   aboutToExit
				*/
				virtual void finalizeWidget(MainControl& main_control)
					throw();	
				
				/// update the state of menu entry Docking
				virtual void checkMenu (MainControl& main_control)
					throw();
				
				
				/// Read the preferences from a INIFile
				void fetchPreferences(INIFile& file)
					throw();
				
				/// Write the preferences to a INIFile
				void writePreferences(INIFile& file)
					throw();
				
					
				/// Reset the dialog to the standard values
				void reset()
					throw();

				/// Calculate...
				bool calculate()
					throw();
				
					
				public slots:
	
					/// Show and raise
					void show();
					
					//
					virtual void genAdvancedPressed();
					
					//
					virtual void evalAdvancedPressed();
					
					//
					virtual void rankAdvancedPressed();
					
					//
					virtual void cancelPressed();
	
					//
					virtual void okPressed();
	
					//
					virtual void resetPressed();
					
					
				protected:
					
				private:
					
					int id_;
					System* selected_system1_;
					System* selected_system2_; 

				
		};
		
} } // Namespaces
#endif
