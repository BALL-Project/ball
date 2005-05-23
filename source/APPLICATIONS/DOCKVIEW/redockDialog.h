#ifndef BALL_VIEW_DIALOGS_REDOCKDIALOG_H
#define BALL_VIEW_DIALOGS_REDOCKDIALOG_H

#ifndef BALL_VIEW_KERNEL_MODULARWIDGET_H
# include <BALL/VIEW/KERNEL/modularWidget.h>
#endif

#ifndef BALL_VIEW_KERNEL_PREFERENCESENTRY_H
# include <BALL/VIEW/KERNEL/preferencesEntry.h>
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

#ifndef BALL_DATATYPE_OPTIONS_H
# include <BALL/DATATYPE/options.h>
#endif

#ifndef BALL_STRUCTURE_DOCKING_CONFORMATIONSET_H
# include <BALL/STRUCTURE/DOCKING/conformationSet.h>
#endif

#include "redockDialogData.h"
#include "dockProgressDialog.h"

namespace BALL
{
	namespace VIEW
	{
		/**	Dialog for redocking to systems.
    		\ingroup  ViewDialogs
		 */
		class BALL_EXPORT RedockDialog : 
			public RedockDialogData,
			public ModularWidget,
			public PreferencesEntry
		{ 
			Q_OBJECT
			BALL_EMBEDDABLE(RedockDialog,ModularWidget)
			
			public:
			
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
				 *	@param      parent the parent widget of the RedockDialog
				 *	@param      name the name of the RedockDialog
				 *	@param			modal the modal flag
				 *	@param			fl the widget flags
				 *	@see        QDialog
				 *	@see        ModularWidget
				 *	@see				PreferncesEntry
				 */
				RedockDialog(QWidget* parent = 0,  const char* name = 0, bool modal = false, WFlags fl = 0)
					throw();
					
				/** Destructor.
				 */
				virtual ~RedockDialog()
					throw();
					
				//@}

				/**  Assignment operator
				 */
				const RedockDialog& operator =(const RedockDialog& redock_dialog)
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
				
				/** Resets the dialog to the standard values.
				 */
				void reset()
					throw();

			
					
				void setSystems(System& s1, System& s2)
					throw();
					
			public slots:
			
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
				
				/** Indicates an algorithm in the combobox was chosen.
				 *	If the chosen algorithm has advanced options, the advanced_button will be enabled.
				 */
				virtual void algorithmChosen();
				
				/** Indicates a scoring function in the combobox was chosen.
				 *	If the chosen scoring function has advanced options, the advanced_button will be enabled.
				 */
				virtual void scoringFuncChosen();
				
			protected:
			
				/** Sets options with values the user has chosen.  
				 */
				void applyValues_() throw();
			
				
			private:
			
				System docking_partner1_;
				System docking_partner2_;
			
				/** key: Algorithm(enum), value: advanced options dialog
				 */
				HashMap<int, QDialog*> algorithm_dialogs_;
				
				/** key: ScoringFunction(enum), value: advanced options dialog
				 */
				HashMap<int, QDialog*> scoring_dialogs_;
			
				/** key: Algorithm(enum)
					* value: vector of scoring functions which can be used with this algorithm
				 */
				HashMap<int, vector<int> > allowed_sf_;
				
				/**
					*/
				HashMap<int, QString> sf_names_;
				
				/** Pointer to docking algorithm
					*/
				DockingAlgorithm* dock_alg_;
					
				//pointer to progress dialog
				DockProgressDialog* progress_dialog_;
				
				/** Options for the docking algorithm
				 */
				Options algorithm_opt_, scoring_opt_;
		};
	} // end of namespace View
} // end of namespace BALL
#endif
