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
 #include <BALL/MOLMEC/COMMON/radiusRuleProcessor.h>
#endif

#ifndef BALL_MOLMEC_COMMON_CHARGERULEPROCESSOR_H
 #include <BALL/MOLMEC/COMMON/chargeRuleProcessor.h>
#endif

#ifndef BALL_STRUCTURE_DEFAULTPROCESSORS_H
 #include <BALL/STRUCTURE/defaultProcessors.h>
#endif

#ifndef BALL_STRUCTURE_FRAGMENTDB_H
 #include <BALL/STRUCTURE/fragmentDB.h>
#endif

#ifndef BALL_DATATYPE_OPTIONS_H
 #include <BALL/DATATYPE/options.h>
#endif

//#include "DockingAlgorithm.h"
#include "geometricFit.h"
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
				
				/** if you want to add a new docking algorithm extend enum  
				*/
				enum Algorithm {GEOMETRIC_FIT = 1, TEST1, TEST2};
				
				/// Constructor
				DockDialog(QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0)
					throw();

				/// Destructor
				virtual ~DockDialog()
					throw();
				
				/**	Initializes the popup menu <b>  Display </b> with its checkable submenu <b>  Docking </b>; 
						This method is called automatically	immediately before the main application is started. 
						@param main_control the  MainControl object to be initialized 
				*/
				virtual void initializeWidget(MainControl& main_control)
					throw();

				/**	Removes the checkable submenu <b>  Docking </b> from the popup menu <b>  Display </b>.
						This method will be called by  MainControl::aboutToExit.
						@param main_control the  MainControl to be finalized 
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

				/// dock the two systems
				bool calculate()
					throw();
					
				/// merge two systems into one
				void mergeSystems()
					throw();
				
				/// Set the systems for docking
				void setSystem(System* system1, System* system2)
					throw() 
				{	docking_partner1_ = system1;
					docking_partner2_ = system2; }
						
				// add docking algorithm to HashMap and ComboBox
				void addEntry(QString name, int algorithm, QDialog* dialog)
					throw();
					
				public slots:
	
					/// Show and raise
					void show();
					
					//
					virtual void algAdvancedPressed();
					
					//
					virtual void rankAdvancedPressed();
					
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
					
					///
					virtual void browseChargesData();

					///
					virtual void browseChargesRules();

					///
					virtual void browseRadiiData();

					///
					virtual void browseRadiiRules();
				
				protected:
					
					bool applyProcessors_() throw();
					
					//set options_ with values user has chosen  
					void applyValues_() throw();
					
					void selectFile_(QLineEdit& lineedit) throw();
					
					//find chosen system
					System* partnerChosen_(QString qstr) throw();
					
					
				private:
					
					// key: Algorithm(enum), value: advanced options dialog
					HashMap<int, QDialog*> algorithm_dialogs_;
				
					//pointer to docking partners
					System* docking_partner1_;
					System* docking_partner2_;
				
					//options for the docking algorithm
					Options 	options_;
					//Docking::DockingAlgorithm docking_;
					
					//menu entry id
					int id_;
					
					//processors
					RadiusRuleProcessor 			radius_rule_processor_;
					ChargeRuleProcessor 			charge_rule_processor_;
					AssignRadiusProcessor 		radius_processor_;
					AssignChargeProcessor 		charge_processor_;
		};
		
} } // Namespaces
#endif
