// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#ifndef BALL_VIEW_DIALOGS_FDPBDIALOG_H
#define BALL_VIEW_DIALOGS_FDPBDIALOG_H

#include <BALL/VIEW/UIC/FDPBDialogData.h>

#ifndef BALL_VIEW_KERNEL_MODULARWIDGET_H
 #include <BALL/VIEW/KERNEL/modularWidget.h>
#endif

#ifndef BALL_SOLVATION_POISSONBOLTZMANN_H
 #include <BALL/SOLVATION/poissonBoltzmann.h>
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

class QLineEdit;

namespace BALL
{
	namespace VIEW
	{

		/** Dialog for performing Finite Distance Poisson Bolzmann calculations
				\ingroup ViewDialogs
		*/
		class BALL_EXPORT FDPBDialog 
		  : public FDPBDialogData,
		    public ModularWidget
		{ 
				Q_OBJECT

				public:
				
				BALL_EMBEDDABLE(FDPBDialog,ModularWidget)

				/// Constructor
				FDPBDialog(QWidget* parent = 0, const char* name = 0, 
									 bool modal = FALSE, WFlags fl = 0);

				/// Destructor
				virtual ~FDPBDialog()
					throw();
					
				///	Read the preferences from a INIFile
				void fetchPreferences(INIFile& file)
					throw();
				
				/// Write the preferences to a INIFile
				void writePreferences(INIFile& file)
					throw();
					
				/// Reset the dialog to the standard values
				void reset()
					throw();

				/// Calculate the FDPB grid
				bool calculate()
					throw();

				/// Get the instance of the FDPB solver
				FDPB& getFDPBSolver()
					throw() {return fdpb_;}

				/// Set the system, the FDPB will be calculated for
				void setSystem(System* system)
					throw() {system_ = system;}

				/// Get the system
				System* getSystem()
					throw() { return system_;}
				

			public slots:

				///
				virtual void browseChargesData();

				///
				virtual void browseChargesRules();

				///
				virtual void browseRadiiData();

				///
				virtual void browseRadiiRules();

				///
				virtual void cancelPressed();

				///
				virtual void okPressed();

				///
				virtual void resetPressed();
				
			protected:

				void selectFile_(QLineEdit& lineedit) throw();
				void applyValues_() throw();
				bool applyProcessors_() throw();
				void fetchPreference_(INIFile& inifile, const String& entry, QLineEdit& lineedit) throw();
				void writePreference_(INIFile& inifile, const String& entry, const QLineEdit& lineedit) const throw();

				FDPB 			fdpb_;
				Options 	options_;
				System* 	system_;
				
				RadiusRuleProcessor 			radius_rule_processor_;
				ChargeRuleProcessor 			charge_rule_processor_;
				AssignRadiusProcessor 		radius_processor_;
				AssignChargeProcessor 		charge_processor_;
		};

} } // Namespaces
#endif
