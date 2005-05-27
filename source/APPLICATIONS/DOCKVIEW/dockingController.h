// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: dockingController.h,v 1.1.2.2 2005/05/27 09:47:55 leonhardt Exp $
//

#ifndef DOCKINGCONTROLLER_H
#define DOCKINGCONTROLLER_H

#ifndef BALL_VIEW_KERNEL_MODULARWIDGET_H
# include <BALL/VIEW/KERNEL/modularWidget.h>
#endif

#ifndef BALL_VIEW_KERNEL_MAINCONTROL_H
# include <BALL/VIEW/KERNEL/mainControl.h>
#endif

#ifndef BALL_FORMAT_INIFILE_H
#include <BALL/FORMAT/INIFile.h>
#endif

#ifndef BALL_VIEW_KERNEL_MESSAGE_H
# include <BALL/VIEW/KERNEL/message.h>
#endif

#ifndef BALL_KERNEL_SYSTEM_H
#include <BALL/KERNEL/system.h>
#endif

#ifndef BALL_STRUCTURE_DOCKING_DOCKINGALGORITHM_H
#include <BALL/STRUCTURE/DOCKING/dockingAlgorithm.h>
#endif

#ifndef BALL_DATATYPE_OPTIONS_H
# include <BALL/DATATYPE/options.h>
#endif

#ifndef BALL_STRUCTURE_DOCKING_CONFORMATIONSET_H
# include <BALL/STRUCTURE/DOCKING/conformationSet.h>
#endif

#include "dockDialog.h"
#include "dockProgressDialog.h"

#include <qwidget.h>

namespace BALL
{
	namespace VIEW
	{
	
		class BALL_EXPORT DockingController
			: public QWidget, 
				public ModularWidget
		{
			Q_OBJECT
			
			public:
			
			BALL_EMBEDDABLE(DockingController, ModularWidget)
			
				////// TODO: enum Algorithm und ScoringFunction in DockingAlgorithm bzw. EnergeticEvaluation ///////
				
				/** if you want to add a new docking algorithm extend enum 
				 *	(0 corresponds to <select> item in ComboBox)
				 */
				enum Algorithm {GEOMETRIC_FIT = 1};
				
				/** if you want to add a new sccoring function extend enum 
				 */
				enum ScoringFunction {DEFAULT = 0, AMBER_FF = 1, RANDOM = 2};
			
				/**	@name	Constructors
				*/	
				//@{

				/** Default Constructor.
						Calls registerWidget.
						\see        ModularWidget
				*/

				DockingController(QWidget* parent = 0, const char* name = 0)
					throw();

				//@}
				/** @name Destructors 
				*/
				//@{

				/** Destructor.
				*/
				virtual ~DockingController()
					throw();
				//@}

				/** Message handling method.
							Catches only ControlSelectionMessage from MolecularControl.
							If such a message is catched the apply button will be enabled and labels
							can be appended onto the selection.
							@param message the pointer to the message that should be processed
				*/
				void onNotify(Message *message)
					throw();

				//@}

				/**	ModularWidget methods
				*/
				//@{

				/**	Initializes the popup menu <b>  Molecular Mechanics </b> with its checkable submenu <b>  Docking </b>;
				*	This method is called automatically	immediately before the main application is started.
				*	@param main_control the  MainControl object to be initialized
				*  @see   openDialog
				*	@see   finalizeWidget
				*	@see   insertMenuEntry
				*/
				void initializeWidget(MainControl& main_control)
					throw();

				/**	Removes the checkable submenu <b>  Docking </b> from the popup menu <b>  Molecular Mechanics </b>.
				*	This method will be called by  MainControl::aboutToExit.
				*	@param main_control the  MainControl to be finalized
				*	@see   initializeWidget
				*	@see   removeMenuEntry
				*	@see   aboutToExit 
				*/
				void finalizeWidget(MainControl& main_control)
					throw();	

				/** Fetches the preferences from the INIFile.
				*	@see    writePreferences
				*/
				void fetchPreferences(INIFile& file)
					throw();

				/** Writes the preferences to the INIFile.
				*  This method will be called inside the method  MainControl::aboutToExit 
				*  @see    fetchPreferences
				*/
				void writePreferences(INIFile& file)
					throw();

				/** Updates the state of menu entry Docking in the popup menu <b>  Molecular Mechanics </b>.
				*/
				void checkMenu (MainControl& main_control)
					throw();	

				//@}	

				DockDialog& getDockDialog()	
					throw();
			
				void runDocking(bool isRedock)
					throw();	
						
			public slots:
				
				void startDocking();
				
			protected:
			
				void runScoring_(ConformationSet* conformation_set)
					throw();
				
			private:
				
				DockDialog dock_dialog_;
				
				/** Pointer to docking algorithm
				*/
				DockingAlgorithm* dock_alg_;
				
				// pointer to progress dialog
				DockProgressDialog* progress_dialog_;
				
				/** Menu entry id
				 */
				int id_;
		};
	}
}
#endif
