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

#include "dockDialogData.h"

namespace BALL
{
	namespace VIEW
	{
		class BALL_EXPORT DockDialog : 
				public DockDialogData,
				public ModularWidget
		{ 
				Q_OBJECT

				public:
				
				/// Constructor
				DockDialog(QWidget* parent = NULL, const char* name = NULL)
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
				
				/** Message handling method.
						Catches only ControlSelectionMessage from MolecularControl.
						If such a message is catched the apply button will be enabled and labels
						can be appended onto the selection.
						@param message the pointer to the message that should be processed
				*/
				/*virtual void onNotify(Message *message)
					throw();*/
				
				virtual void checkMenu (MainControl& main_control)
					throw();
				
				public slots:
				
  			/** @name Public slots
	  		*/
				//@{
				
				/** Show and raise the dialog
				*/
				void show();
						
					
						
				private:
					
					int id_;
					System* selected_system1_;
					System* selected_system2_; 
				
		};

} } // Namespaces
#endif
