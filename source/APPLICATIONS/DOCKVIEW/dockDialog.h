//   // -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_DIALOGS_DOCKDIALOG_H
#define BALL_VIEW_DIALOGS_DOCKDIALOG_H

#ifndef BALL_VIEW_KERNEL_PREFERENCESENTRY_H
# include <BALL/VIEW/KERNEL/preferencesEntry.h>
#endif

#ifndef BALL_FORMAT_INIFILE_H
#include <BALL/FORMAT/INIFile.h>
#endif

#ifndef BALL_KERNEL_SYSTEM_H
#include <BALL/KERNEL/system.h>
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

#ifndef BALL_DATATYPE_OPTIONS_H
# include <BALL/DATATYPE/options.h>
#endif

#include "dockDialogData.h"
#include "dockResult.h"
#include "dockProgressDialog.h"
//#include "dockingController.h"


namespace BALL
{
	namespace VIEW
	{
		class DockingController;
	
		/**	Dialog for docking/redocking two systems.
    		\ingroup  ViewDialogs
		 */
		class BALL_EXPORT DockDialog : 
			public DockDialogData,
			public PreferencesEntry
		{ 
			Q_OBJECT
			
			public:
			
				/**	@name	Constructors and Destructors
				 */	
				//@{
			
				/** Default Constructor.
				 *	Calls  \link PreferencesEntry::registerObject_ registerObject_ \endlink.
				 *	Sets flag is_redock_ to false.
				 *	@param      parent the parent widget of the DockDialog
				 *	@param      name the name of the DockDialog
				 *	@param			modal the modal flag
				 *	@param			fl the widget flags
				 *	@see        QDialog
				 *	@see				PreferncesEntry
				 */
				DockDialog(QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0)
					throw();
					
				/** Copy constructor.
					*/
				DockDialog(const DockDialog& dock_dialog)
					throw();
					
				/** Destructor.
				 */
				virtual ~DockDialog()
					throw();
				//@}

				/**	@name	Assignment
				 */	
				//@{
				
				/**  Assignment operator
				 */
				const DockDialog& operator =(const DockDialog& dock_dialog)
					throw();
				//@}
					
				/**	@name	Accessors: inspectors and mutators
			 	 */
				//@{
				
				/** Sets two systems as docking partners.
				 *	@param      system1 first docking partner
				 *	@param      system2 second docking partner
				 */
				void setSystems(System* system1, System* system2)
					throw();
				
				/**  Get first docking partner.
				 */
				System* getSystem1()
					throw();
				
				/**  Get second docking partner.
				 */
				System* getSystem2()
					throw();
				
				/** Get algorithm options.
				 */
				Options& getAlgorithmOptions()
					throw();
				
				/** Get scoring options.
				 */
				Options& getScoringOptions()
					throw();
				
				/** Sets the flags 'is_redock_' and 'has_changed_'.
				 */
				void isRedock(bool is_redock)
					throw();
					
				/** Adds docking algorithm to Combobox and its advanced option dialog to HashMap.
				 *	@param      name the name of the algorithm
				 *	@param      algorithm the value of enum DockingController::Algorithm
				 *	@param      dialog pointer to an advanced option dialog
				 */
				//void addAlgorithm(const QString& name, DockingController::Algorithm algorithm, QDialog* dialog)
				void addAlgorithm(const QString& name, const int algorithm, QDialog* dialog)
					throw();
					
				/** Adds scoring function to Combobox and its advanced option dialog to HashMap, if it has such a dialog.
				 *	@param      name the name of the scoring function
				 *	@param      score_func the value of enum DockingController::ScoringFunction
				 *	@param      dialog pointer to an advanced option dialog
				 */
				//void addScoringFunction(const QString& name, DockingController::ScoringFunction score_func, QDialog* dialog=0)
				void addScoringFunction(const QString& name, const int score_func, QDialog* dialog=0)
					throw();
				
				/**	Builds HashMaps for algorithm advanced option dialogs and for scoring function advanced option dialogs.
				 *	Also builds HashMap with the allowed scoring functions for the different algorithms.
				 *  Is called by \link DockingController::initializeWidget initializeWidget \endlink.
				 *	@see				addAlgorithm
				 *	@see				addScoringFunction
				 */
				void initializeWidget()
					throw();

				/** Fetchs the preferences from the INIFile.
					* Calls \link PreferencesEntry::readPreferenceEntries readPreferenceEntries \endlink.
					* Calls \link fetchPreferences_ fetchPreferences_ \endlink to read the redocking options.
					* This method is called in \link DockingController::fetchPreferences fetchPreferences \endlink.
				 	*	@see    writePreferences
				 	*/
				void fetchPreferences(INIFile& file)
					throw();
				
				/** Writes the preferences to the INIFile.
					* Calls \link PreferencesEntry::writePreferenceEntries writePreferenceEntries \endlink.
				  * This method is called in \link DockingController::writePreferences writePreferences \endlink.
				  * @see    fetchPreferences
				  */
				void writePreferences(INIFile& file)
					throw();
				
					
				/** Resets the dialog to the standard values.
				 */
				void reset()
					throw();
				//@}
					
			public slots:
	
				/** Shows and raises the dialog.
					* Dialog is adapted for docking / redocking.
				 *	Calls \link fillSystemComboboxes_ fillSystemComboboxes_ \endlink in case of docking.
				 */
				void show();
				
				/** Is called when reset button is pressed.
				 * Calls \link DockDialog::reset reset \endlink.
				 */
				void resetPressed();
			
				/** Is called when OK button is pressed.
				 * 	For docking, it checks if two different systems are chosen.
				 *  Hides the dialog and calls \link DockDialog::applyValues_ applyValues_ \endlink and \link DockDialog::applyProcessors_ applyProcessors_ \endlink.
				 */
				void okPressed();
				
				/** Is called when cancel button is pressed.
				 *	Hides the dialog.
				 */
				void cancelPressed();
				
				/** Is called when advanced button for algorithm options is pressed.
			 	 *	Shows corresponding options dialog.
				 */
				void algAdvancedPressed();
				
				/** Is called when advanced button for scoring function options is pressed.
			 	 *	Shows corresponding options dialog, if it exits.
			 	 */
				void scoringAdvancedPressed();
				
				/** Is called when a system in the combobox is chosen as docking partner 1.
				 *  Calls \link DockDialog::partnerChosen_ partnerChosen_ \endlink.
				 */
				void partner1Chosen();
				
				/** Is called when a system in the combobox is chosen as docking partner 2.
				 *  Calls \link DockDialog::partnerChosen_ partnerChosen_ \endlink.
				 */
				void partner2Chosen();
				
				/** Is called when a scoring function in the combobox is chosen.
				 *	If the chosen scoring function has advanced options, the advanced_button will be enabled.
				 */
				void scoringFuncChosen();
				
				/** Is called when an algorithm in the combobox is chosen.
				 *	If the chosen algorithm has advanced options, the advanced_button will be enabled.
				 */
				void algorithmChosen();
				
				/** Is called when browse button to get a charges config file from table is pressed.
					*  Calls \link DockDialog::selectFile_ selectFile_ \endlink.
				 */
				void browseChargesData();
				
				/** Is called when browse button to get a charges config file by rules is pressed.
				 *  Calls \link DockDialog::selectFile_ selectFile_ \endlink.
				 */
				void browseChargesRules();
				
				/** Is called when browse button to get a radii config file from table is pressed.
				 *  Calls \link DockDialog::selectFile_ selectFile_ \endlink.
				 */
				void browseRadiiData();
				
				/** Is called when browse button to get a radii config file by rules s pressed.
				 *  Calls \link DockDialog::selectFile_ selectFile_ \endlink.
				 */
				void browseRadiiRules();
				
				
			protected:
			
				/** Sets options algorithm_opt_ and scoring_opt_ with values the user has chosen.  
				 */
				void applyValues_() throw();
				
				/** Applies processors to the systems.
				 */
				bool applyProcessors_() throw();
				
				/** Shows chosen file in the dialog.
				 */
				void selectFile_(QLineEdit& lineedit) throw();
				
				/** Get system which the user has chosen in the dialog as docking partner.
					* @param				qstr name of the docking partner
				 */
				System* partnerChosen_(const QString& qstr) throw();
				
				/** Fills the system comboboxes.
				 *  If the user has already selected one or two systems, they are the current items in the comboboxes. 
				 */
				void fillSystemComboboxes_() throw();
				
				/** Reads the redocking values from INIFile into vector backup_.
				 	*	If INIFile has not yet a section REDOCKING, fill backup_ vector with default values.
				 	*	@param    	file the INIFile that is read
					*	@param     	entry key of entry that is read
					*	@param    	default_value default value
					* @see				fetchPreferences
				 */
				void fetchPreferences_(INIFile& file, const String& entry, const QString& default_value) throw();
			
				/** Swaps the option values between vector backup_ and dialog.
				 *  Is called in \link DockDialog::show show \endlink if has_changed_ is true
				 *  and in \link DockDialog::writePreferences writePreferences \endlink if is_redock_ is true
				 */
				void swapValues_() throw();
				
				
			private:
				
				/** Flag which indicates if we do docking or redocking.
					*/
				bool is_redock_;
				
				/** Flag:
					* True if we do docking and did redocking before or otherwise.
					* False if we do (re)docking and also did (re)docking before.
					*/
				bool has_changed_;
			
				/** key: DockingController::Algorithm
				 *  value: advanced options dialog
				 */
				HashMap<int, QDialog*> algorithm_dialogs_;
				
				/** key: DockingController::ScoringFunction
				 *  value: advanced options dialog
				 */
				HashMap<int, QDialog*> scoring_dialogs_;
			
				/** key: DockingController::Algorithm
				 *  value: vector of scoring functions which can be used with this algorithm
				 */
				HashMap<int, vector<int> > allowed_sf_;
				
				/** Pointer to docking partner 1
				 */
				System* docking_partner1_;
				
				/** Pointer to docking partner 2
				 */
				System* docking_partner2_;	
				
				/** Options for the docking algorithm and scoring function
				 */
				Options algorithm_opt_, scoring_opt_;
		
				/** Needed to guarantee that both, docking and redocking preferences can be written to INIFile
					* When we do docking, redocking values are in the vector and when we do redocking, the docking values are in there.
					* In \link DockDialog::fetchPreferences fetchPreferences \endlink, we read the last redocking values from INIFile in this vector
					* and in \link DockDialog::writePreferences writePreferences \endlink, we write the redocking values in INIFile from this vector
					*/
				vector<QString> backup_;
				
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
