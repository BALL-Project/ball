//
// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_DIALOGS_DOCKDIALOG_H
#define BALL_VIEW_DIALOGS_DOCKDIALOG_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#ifndef BALL_VIEW_KERNEL_PREFERENCESENTRY_H
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

#ifndef BALL_DATATYPE_OPTIONS_H
# include <BALL/DATATYPE/options.h>
#endif

#include <BALL/VIEW/UIC/ui_dockDialog.h>

namespace BALL
{
	class INIFile;
	class System;
	class Options;

	namespace VIEW
	{
		class DockingController;
		
	
		/**	Dialog for docking / redocking two systems. 
		  * In this dialog the user choose the two docking partners, the docking algorithm and the scoring function.
			* Furthermore he can set the options for the algorithm and scroing function and can apply processors to the docking parnters.
			* For redocking he can specify the euler angles.
    		\ingroup  ViewDialogs
		 */
		class BALL_VIEW_EXPORT DockDialog : 
			public QDialog,
			public Ui_DockDialogData,
			public PreferencesEntry
		{ 
			Q_OBJECT
			
			public:
		
				/**	@name	Constructors and Destructors
				 */	
				//@{
			
				/** Default Constructor.
				 *	Calls  \link PreferencesEntry::registerObject_ PreferencesEntry::registerObject_ \endlink.
				 *	Sets flag \link DockDialog::is_redock_ is_redock_ \endlink to false.
				 *	@param      parent the parent widget of the DockDialog
				 *	@param      name the name of the DockDialog
				 *	@param			modal the modal flag
				 *	@param			fl the widget flags
				 *	@see        QDialog
				 *	@see				PreferncesEntry
				 */
				DockDialog(QWidget* parent = 0, const char* name = "DockDialog");
					
				/** Destructor.
				 */
				virtual ~DockDialog();
				//@}

				/**	@name	Assignment
				 */	
				//@{
				
				/**  Assignment operator
				 */
				const DockDialog& operator =(const DockDialog& dock_dialog);
				//@}
					
				/**	@name	Accessors: inspectors and mutators
			 	 */
				//@{
				
				/** Sets two systems as docking partners.
				 *	@param      system1 first docking partner
				 *	@param      system2 second docking partner
				 */
				void setSystems(System* system1, System* system2);
				
				/**  Get first docking partner.
				 */
				System* getSystem1();
				
				/**  Get second docking partner.
				 */
				System* getSystem2();
				
				/** Get algorithm options.
				 */
				Options& getAlgorithmOptions();
				
				/** Get scoring options.
				 */
				Options& getScoringOptions();
				
				/** Sets the flags \link DockDialog::is_redock_ is_redock_ \endlink and \link DockDialog::has_changed_ has_changed_ \endlink.
				 */
				void isRedock(bool is_redock);
					
				/** Adds docking algorithm to combobox and its advanced option dialog to hashmap.
				 *	@param      name the name of the algorithm
				 *	@param      algorithm the value of enum DockingController::Algorithm
				 *	@param      dialog pointer to an advanced option dialog
				 */
				// remark: score_func isn't enum type because we then have cyclic includes of DockDialog and DockingController
				void addAlgorithm(const QString& name, const int algorithm, QDialog* dialog);
					
				/** Adds scoring function to combobox and its advanced option dialog to hashmap, if it has such a dialog.
				 *	@param      name the name of the scoring function
				 *	@param      score_func the value of enum DockingController::ScoringFunction
				 *	@param      dialog pointer to an advanced option dialog
				 */
				// remark: score_func isn't enum type because we then have cyclic includes of DockDialog and DockingController
				void addScoringFunction(const QString& name, const int score_func, QDialog* dialog=0);
				
				/**	Builds hashmaps for algorithm advanced option dialogs and for scoring function advanced option dialogs.
				 *	Also builds hashmap with the allowed scoring functions for the different algorithms.
				 *  Is called by \link DockingController::initializeWidget DockingController::initializeWidget \endlink.
				 *	@see				addAlgorithm
				 *	@see				addScoringFunction
				 */
				virtual void initializeWidget();

				/** Fetchs the preferences from the INIFile.
					* Calls \link PreferencesEntry::readPreferenceEntries PreferencesEntry::readPreferenceEntries \endlink.
					* Calls \link DockDialog::fetchPreferences_ fetchPreferences_ \endlink to read the redocking options.
					* This method is called in \link DockingController::fetchPreferences DockingController::fetchPreferences \endlink.
				 	*	@see    writePreferences
				 	*/
				virtual void fetchPreferences(INIFile& file);
				
				/** Writes the preferences to the INIFile.
					* Calls \link PreferencesEntry::writePreferenceEntries PreferencesEntry::writePreferenceEntries \endlink.
				  * This method is called in \link DockingController::writePreferences DockingController::writePreferences \endlink.
				  * @see    fetchPreferences
				  */
				virtual void writePreferences(INIFile& file);
				
					
				/** Resets the dialog to the standard values.
				 */
				void reset();
				//@}
					
			public slots:
	
				/** Shows and raises the dialog.
					* Dialog is adapted for docking / redocking.
				 *	Calls \link fillSystemComboboxes_ fillSystemComboboxes_ \endlink in case of docking.
				 */
				int exec();
				
				/** Is called when reset button is pressed.
				 * Calls \link DockDialog::reset reset \endlink.
				 */
				void resetPressed();
			
				/** Is called when OK button is pressed.
				 * 	For docking, it checks if two different systems are chosen.
				 *  Hides the dialog and calls \link DockDialog::applyValues_ applyValues_ \endlink
				 *  and \link DockDialog::applyProcessors_ applyProcessors_ \endlink.
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
			
				/** Sets options \link DockDialog::algorithm_opt_ algorithm_opt_ \endlink and 
				  * \link DockDialog::scoring_opt_ scoring_opt_ \endlink with values the user has chosen.  
				 */
				void applyValues_();
				
				/** Applies processors to the systems.
				 */
				bool applyProcessors_();
				
				/** Shows chosen file in the dialog.
				 */
				void selectFile_(QLineEdit& lineedit);
					
				/** Fills the system comboboxes.
				 *  If the user has already selected one or two systems, they are the current items in the comboboxes. 
				 */
				void fillSystemComboboxes_();
				
				/** Reads the redocking values from INIFile into vector backup_.
				 	*	If INIFile has not yet a section <b> REDOCKING </b>, fill vector \link DockDialog::backup_ backup_ \endlink with default values.
				 	*	@param    	file the INIFile that is read
					*	@param     	entry key of entry that is read
					*	@param    	default_value default value
					* @see				fetchPreferences
				 */
				void fetchPreferences_(INIFile& file, const String& entry, const QString& default_value);
			
				/** Swaps the option values between vector backup_ and dialog.
				 *  Is called in \link DockDialog::show show \endlink if \link DockDialog::has_changed_ has_changed_ \endlink is true
				 *  and in \link DockDialog::writePreferences writePreferences \endlink if \link DockDialog::is_redock_ is_redock_ \endlink is true
				 */
				void swapValues_();
				
				
			private:

				/** Copy constructor.
					* Remark: Copy contructor is private because it is not completed. 
					* The copy constuctor of the QT widgets is private and cannot be called.  
					*/
				DockDialog(const DockDialog& dock_dialog);
			
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
				
			  /** vector contains pointers to all systems that are loaded into BALLView
			   */
			  vector<System*> loaded_systems_;

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
