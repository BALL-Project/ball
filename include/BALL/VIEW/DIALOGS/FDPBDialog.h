// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#ifndef BALL_VIEW_DIALOGS_FDPBDIALOG_H
#define BALL_VIEW_DIALOGS_FDPBDIALOG_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#ifndef BALL_VIEW_KERNEL_MODULARWIDGET_H
# include <BALL/VIEW/KERNEL/modularWidget.h>
#endif

#ifndef BALL_VIEW_KERNEL_PREFERENCESENTRY
# include <BALL/VIEW/KERNEL/preferencesEntry.h>
#endif

#ifndef BALL_SOLVATION_POISSONBOLTZMANN_H
# include <BALL/SOLVATION/poissonBoltzmann.h>
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

#include <BALL/VIEW/UIC/ui_FDPBDialog.h>

class QLineEdit;

namespace BALL
{
	namespace VIEW
	{

		class CalculateFDPBThread;

		/** Dialog for performing Finite Distance Poisson Bolzmann calculations
				\ingroup ViewDialogs
		*/
		class BALL_VIEW_EXPORT FDPBDialog 
		  : public QDialog,
				public Ui_FDPBDialogData,
		    public ModularWidget,
				public PreferencesEntry
		{ 
			friend class CalculateFDPBThread;

			Q_OBJECT

			public:
			
			BALL_EMBEDDABLE(FDPBDialog,ModularWidget)

			/// Constructor
			FDPBDialog(QWidget* parent = 0, const char* name = "FDPBDialog",
								 bool modal = FALSE, Qt::WFlags fl = 0);

			/// Destructor
			virtual ~FDPBDialog();
				
			/// Calculate the FDPB grid
			bool calculate();

			/// Get the instance of the FDPB solver
			FDPB& getFDPBSolver()
				{return fdpb_;}

			/// Set the system, the FDPB will be calculated for
			void setSystem(System* system)
				{system_ = system;}

			/// Get the system
			System* getSystem()
				{ return system_;}
			

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
			virtual void accept();

			///
			virtual void resetPressed();

		protected slots:
			virtual void clicked(QAbstractButton* button);

		protected:

			void calculate_();
			void selectFile_(QLineEdit& lineedit);
			void applyValues_();
			bool applyProcessors_();

			FDPB 			fdpb_;
			Options 	options_;
			System* 	system_;
			
			RadiusRuleProcessor 			radius_rule_processor_;
			ChargeRuleProcessor 			charge_rule_processor_;
			AssignRadiusProcessor 		radius_processor_;
			AssignChargeProcessor 		charge_processor_;
			CalculateFDPBThread* 			thread_;
		};

} } // Namespaces
#endif
