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

#include "dockResultDialog.h"
#include "dockDialogData.h"
#include "dockResult.h"

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
			BALL_EMBEDDABLE(DockDialog,ModularWidget)
			
			public:
				
				////// ToDo: enum Algorithm und ScoringFunction in DockingAlgorithm bzw. EnergeticEvaluation ///////
				
				/** if you want to add a new docking algorithm extend enum 
						(0 corresponds to <select> item in ComboBox)
				*/
				enum Algorithm {GEOMETRIC_FIT = 1};
				
				/** if you want to add a new sccoring function extend enum 
				*/
				enum ScoringFunction {DEFAULT = 0, RANDOM = 1};
				
				// Constructor
				DockDialog(QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0)
					throw();

				// Destructor
				virtual ~DockDialog()
					throw();
				
				/**  Assignment operator
				*/
				const DockDialog& operator =(const DockDialog& dock_dialog)
					throw();
				
				// add docking algorithm to HashMap and ComboBox
				void addAlgorithm(const QString& name, const int algorithm, QDialog* dialog)
					throw();
					
				// add scoring function to HashMap and ComboBox
				void addScoringFunction(const QString& name, const int score_func, QDialog* dialog=0)
					throw();
					
					// Set the systems for docking
				void setSystem(System* system1, System* system2)
					throw();
				
				/**	Initializes the popup menu <b>  Molecular Mechanics </b> with its checkable submenu <b>  Docking </b>; 
						This method is called automatically	immediately before the main application is started. 
						@param main_control the  MainControl object to be initialized 
				*/
				virtual void initializeWidget(MainControl& main_control)
					throw();

				/**	Removes the checkable submenu <b>  Docking </b> from the popup menu <b>  Molecular Mechanics </b>.
						This method will be called by  MainControl::aboutToExit.
						@param main_control the  MainControl to be finalized 
				*/
				virtual void finalizeWidget(MainControl& main_control)
					throw();	
				
				// update the state of menu entry Docking
				virtual void checkMenu (MainControl& main_control)
					throw();
				
				// Read the preferences from a INIFile
				void fetchPreferences(INIFile& file)
					throw();
				
				// Write the preferences to a INIFile
				void writePreferences(INIFile& file)
					throw();
				
				// Reset the dialog to the standard values
				void reset()
					throw();

				// dock the two systems
				bool calculate()
					throw();
					
					
			public slots:
	
				/// Show and raise dock dialog
				void show();
				
				// advanced button for algorithm options pressed
				virtual void algAdvancedPressed();
				
				// advanced button for scoring function options pressed
				virtual void scoringAdvancedPressed();
				
				//
				virtual void cancelPressed();
				
				//
				virtual void okPressed();
				
				//
				virtual void resetPressed();
				
				//
				virtual void partner1Chosen();
				
				//
				virtual void partner2Chosen();
				
				//
				virtual void algorithmChosen();
				
				//
				virtual void scoringFuncChosen();
				
				///
				virtual void browseChargesData();
				
				///
				virtual void browseChargesRules();
				
				///
				virtual void browseRadiiData();
				
				///
				virtual void browseRadiiRules();
				
				
			protected:
				
				//set options_ with values user has chosen  
				void applyValues_() throw();
				
				// apply the processors to the systems
				bool applyProcessors_() throw();
				
				// show chosen file in the dialog
				void selectFile_(QLineEdit& lineedit) throw();
				
				// get system which the user has chosen in the dialog as docking partner
				System* partnerChosen_(QString qstr) throw();
				
				
			private:
				
				// key: Algorithm(enum), value: advanced options dialog
				HashMap<int, QDialog*> algorithm_dialogs_;
				
				// key: ScoringFunction(enum), value: advanced options dialog
				HashMap<int, QDialog*> scoring_dialogs_;
			
				//pointer to docking partners
				System* docking_partner1_;
				System* docking_partner2_;
			
				//options for the docking algorithm
				Options options_;
				
				//menu entry id
				int id_;
				
				//processors
				RadiusRuleProcessor 		radius_rule_processor_;
				ChargeRuleProcessor 		charge_rule_processor_;
				AssignRadiusProcessor 	radius_processor_;
				AssignChargeProcessor 	charge_processor_;
		};
			
} } // Namespaces
#endif
