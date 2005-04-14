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

#ifndef BALL_MOLMEC_COMMON_RADIUSRULEPROCESSOR_H
# include <BALL/MOLMEC/COMMON/radiusRuleProcessor.h>
#endif

#ifndef BALL_MOLMEC_COMMON_CHARGERULEPROCESSOR_H
# include <BALL/MOLMEC/COMMON/chargeRuleProcessor.h>
#endif

#ifndef BALL_STRUCTURE_DEFAULTPROCESSORS_H
# include <BALL/STRUCTURE/defaultProcessors.h>
#endif

#ifndef BALL_STRUCTURE_FRAGMENTDB_H
# include <BALL/STRUCTURE/fragmentDB.h>
#endif

#ifndef BALL_DATATYPE_OPTIONS_H
# include <BALL/DATATYPE/options.h>
#endif

#ifndef BALL_STRUCTURE_DOCKING_DOCKINGALGORITHM_H
# include <BALL/STRUCTURE/DOCKING/dockingAlgorithm.h>
#endif

#ifndef BALL_STRUCTURE_DOCKING_GEOMETRICFIT_H
# include <BALL/STRUCTURE/DOCKING/geometricFit.h>
#endif

#include "dockDialogData.h"
#include "dockResult.h"
#include "dockProgressDialog.h"

namespace BALL
{
	namespace VIEW
	{
		/**	Dialog for docking to systems.
    		\ingroup  ViewDialogs
		 */
		class BALL_EXPORT DockDialog : 
			public DockDialogData,
			public ModularWidget,
			public PreferencesEntry
		{ 
			Q_OBJECT
			BALL_EMBEDDABLE(DockDialog,ModularWidget)
			
			public:
				
				////// TODO: enum Algorithm und ScoringFunction in DockingAlgorithm bzw. EnergeticEvaluation ///////
				
				/** if you want to add a new docking algorithm extend enum 
				 *	(0 corresponds to <select> item in ComboBox)
				 */
				enum Algorithm {GEOMETRIC_FIT = 1};
				
				/** if you want to add a new sccoring function extend enum 
				 */
				enum ScoringFunction {DEFAULT = 0, AMBER_FF = 1, RANDOM = 2};
				
				/**	@name	Constructors and Destructors
				 */	
				//@{
			
				/** Default Constructor.
				 *	Calls  \link ModularWidget::registerWidget registerWidget \endlink and \link PreferencesEntry::registerObject_ registerObject_ \endlink 
				 *	Builds HashMaps for algorithm advanced option dialogs and for scoring function advanced option dialogs.
				 *	@param      parent the parent widget of the DockDialog
				 *	@param      name the name of the DockDialog
				 *	@param			modal the modal flag
				 *	@param			fl the widget flags
				 *	@see        QDialog
				 *	@see        ModularWidget
				 *	@see				PreferncesEntry
				 */
				DockDialog(QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0)
					throw();

				/** Destructor.
				 */
				virtual ~DockDialog()
					throw();

				//@}

				/**  Assignment operator
				 */
				const DockDialog& operator =(const DockDialog& dock_dialog)
					throw();
					
				/**	@name	Accessors: inspectors and mutators
			 	 */
				//@{
				
				/** Sets two systems as docking partners.
				 *	@param      system1 first docking partner
				 *	@param      system2 second docking partner
				 */
				void setSystem(System* system1, System* system2)
					throw();
				
				/** Adds docking algorithm to Combobox and its advanced option dialogs to HashMap.
				 *	@param      name the name of the algorithm
				 *	@param      algorithm the value of enum Algorithm
				 *	@param      dialog pointer to an advanced option dialog
				 */
				void addAlgorithm(const QString& name, const int algorithm, QDialog* dialog)
					throw();
					
				/** Adds scoring function to Combobox and its advanced option dialogs to HashMap, if it has such an dialog.
				 *	@param      name the name of the scoring function
				 *	@param      score_func the value of enum ScoringFunction
				 *	@param      dialog pointer to an advanced option dialog
				 */
				void addScoringFunction(const QString& name, const int score_func, QDialog* dialog=0)
					throw();
				
				/** Message handling method.
						Catches only ControlSelectionMessage from MolecularControl.
						If such a message is catched the apply button will be enabled and labels
						can be appended onto the selection.
						@param message the pointer to the message that should be processed
		  	*/
				virtual void onNotify(Message *message)
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
				virtual void initializeWidget(MainControl& main_control)
					throw();

				/**	Removes the checkable submenu <b>  Docking </b> from the popup menu <b>  Molecular Mechanics </b>.
				 *	This method will be called by  MainControl::aboutToExit.
				 *	@param main_control the  MainControl to be finalized
				 *	@see   initializeWidget
				 *	@see   removeMenuEntry
				 *	@see   aboutToExit 
				 */
				virtual void finalizeWidget(MainControl& main_control)
					throw();	
				
				/** Fetches the preferences from the INIFile.
				 *	@see    writePreferences
				 */
				virtual void fetchPreferences(INIFile& file)
					throw();
				
				/** Writes the preferences to the INIFile.
				 *  This method will be called inside the method  MainControl::aboutToExit 
				 *  @see    fetchPreferences
				 */
				virtual void writePreferences(INIFile& file)
					throw();
				
				/** Updates the state of menu entry Docking in the popup menu <b>  Molecular Mechanics </b>.
				 */
				virtual void checkMenu (MainControl& main_control)
					throw();	
				
				//@}	
					
				/** Resets the dialog to the standard values.
				 */
				void reset()
					throw();

				/** Docks the two systems.
				 * Calls \link DockDialog::applyValues_ applyValues_ \endlink.
				 * Calls \link DockDialog::applyProcessors_ applyProcessors_ \endlink.
				 */
				void calculate()
					throw();
					
					
			public slots:
	
				/** Shows and raises the dialog.
				 *	The comboboxes for the docking partners are filled with the loaded systems in BALLView.
				 *	If the user has selected one or two systems, they are the current items in the comboboxes.
				 */
				void show();
				
				/** Indicates the ok button was pressed.
				 * 	Checks if two different systems are chosen. Hides the dialog and calls \link DockDialog::calculate calculate \endlink .
				 */
				virtual void okPressed();
				
				/** Indicates the cancel button was pressed.
				 *	Hides dialog.
				 */
				virtual void cancelPressed();
				
				/** Indicates the reset button was pressed.
				 * Calls \link DockDialog::reset reset \endlink.
				 */
				virtual void resetPressed();
				
				/** Indicates the advanced button for algorithm option button was pressed.
			 	 *	Show corresponding options dialog.
				 */
				virtual void algAdvancedPressed();
				
				/** Indicates the advanced button for algorithm option button was pressed.
			 	 *	Show corresponding options dialog, if it exits.
			 	 */
				virtual void scoringAdvancedPressed();
				
				/** Indicates a system in the combobox was chosen as docking partner 1.
				 *  Calls \link DockDialog::partnerChosen_ partnerChosen_ \endlink.
				 */
				virtual void partner1Chosen();
				
				/** Indicates a system in the combobox was chosen as docking partner 2.
				 *  Calls \link DockDialog::partnerChosen_ partnerChosen_ \endlink.
				 */
				virtual void partner2Chosen();
				
				/** Indicates a scoring function in the combobox was chosen.
				 *	If the chosen scoring function has advanced options, the advanced_button will be enabled.
				 */
				virtual void scoringFuncChosen();
				
				/** Indicates the browse button to get a charges config file from table was pressed.
				 */
				virtual void browseChargesData();
				
				/** Indicates the browse button to get a charges config file by rules was pressed.
				 *  Calls \link DockDialog::selectFile_ selectFile_ \endlink.
				 */
				virtual void browseChargesRules();
				
				/** Indicates the browse button to get a radii config file from table was pressed.
				 *  Calls \link DockDialog::selectFile_ selectFile_ \endlink.
				 */
				virtual void browseRadiiData();
				
				/** Indicates the browse button to get a radii config file by rules was pressed.
				 *  Calls \link DockDialog::selectFile_ selectFile_ \endlink.
				 */
				virtual void browseRadiiRules();
			
				
				protected slots:
				
				void continueCalculate_(ConformationSet* conformation_set) throw();	
			
			protected:
			
				/** Sets options_ with values the user has chosen.  
				 */
				void applyValues_() throw();
				
				/** Apply processors to the systems.
				 */
				bool applyProcessors_() throw();
				
				/** Shows chosen file in the dialog.
				 */
				void selectFile_(QLineEdit& lineedit) throw();
				
				/** Get system which the user has chosen in the dialog as docking partner.
				 */
				System* partnerChosen_(QString qstr) throw();
				
			private:
				
				/** key: Algorithm(enum), value: advanced options dialog
				 */
				HashMap<int, QDialog*> algorithm_dialogs_;
				
				/** key: ScoringFunction(enum), value: advanced options dialog
				 */
				HashMap<int, QDialog*> scoring_dialogs_;
			
				/** Pointer to docking partners
				 */
				System* docking_partner1_;
				System* docking_partner2_;
				
				//pointer to progress dialog
				DockProgressDialog* progress_dialog_;
			
				/** Options for the docking algorithm
				 */
				Options options_;
				
				/** Menu entry id
				 */
				int id_;
				
				/** Processors
				 */
				RadiusRuleProcessor 		radius_rule_processor_;
				ChargeRuleProcessor 		charge_rule_processor_;
				AssignRadiusProcessor 	radius_processor_;
				AssignChargeProcessor 	charge_processor_;
		};
			
	} // end of namespace View
} // end of namespace BALL
#endif
