// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_DIALOGS_MINIMIZATIONDIALOG_H
#define BALL_VIEW_DIALOGS_MINIMIZATIONDIALOG_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#ifndef BALL_VIEW_KERNEL_PREFERENCESENTRY
# include <BALL/VIEW/KERNEL/preferencesEntry.h>
#endif

#include <BALL/VIEW/UIC/ui_minimizationDialog.h>

namespace BALL
{
	namespace VIEW
	{
		class AmberConfigurationDialog;
		class CharmmConfigurationDialog;
		class MMFF94ConfigurationDialog;

		/** Dialog for performing energy minimisations
				\ingroup ViewDialogs
		*/
		class BALL_VIEW_EXPORT MinimizationDialog 
			: public QDialog,
				public Ui_MinimizationDialogData,
				public PreferencesEntry
		{
				Q_OBJECT

				public:

				/// Constructor
				MinimizationDialog(QWidget* parent = NULL, const char* name = "MinimizationDialog");

				/// Destructor
				virtual ~MinimizationDialog();

				/// Get the maximum number of iterations of the minimizer.
				Size getMaxIterations() const;

				/// Set the maximum number of iterations for the minimizer.
				void setMaxIterations(Size n);
			
				/// Get the number of steps between the Scene refresh.
				Size getRefresh() const;
				
				/// Set the number of steps between the Scene refreshs.
				void setRefresh(Size n);
			
				///
				double getMaxGradient() const;
			
				///
				void setMaxGradient(double max_gradient);
			
				///
				double getEnergyDifference() const;
			
				///
				void setEnergyDifference(double energy_difference);
				
				///
				bool getUseStrangLBFGS() const;

				///
				void setUseStrangLBFGS(bool use_LBFGS);

				///
				bool getUseShiftedLVMM() const;

				///
				void setUseShiftedLVMM(bool use_shifted);

				///
				bool getUseConjugateGradient() const;
				
				///
				void setUseConjugateGradient(bool use_CG);
				
				/** Set the dialog for the AMBER Configuration.
						Called by MolecularStructure.
				*/
				void setAmberDialog(AmberConfigurationDialog* amber_dialog);
				
				/** Set the dialog for the CHARMM Configuration.
						Called by MolecularStructure.
				*/
				void setCharmmDialog(CharmmConfigurationDialog* charmm_dialog);
				
				///
				void setMMFF94Dialog(MMFF94ConfigurationDialog* dialog);
		
				/// Select the forcefield, nr are the enum values in MolecularStructure
				void selectForceField(Position nr);
				
				/// Return the ID of the selected forcefield (see enum values in MolecularStructure)
				Position selectedForceField() const;
				
				public slots:

				///
				virtual void accept();

				/// Show an dialog for setting the options of the currently selected force field.
				void advancedOptions();
				
				private:
				AmberConfigurationDialog* amber_dialog_;
				CharmmConfigurationDialog* charmm_dialog_;
				MMFF94ConfigurationDialog* mmff_dialog_;
		};
		
	} // namespace VIEW
} // namespace BALL

#endif
