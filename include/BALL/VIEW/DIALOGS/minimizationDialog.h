// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_DIALOGS_MINIMIZATIONDIALOG_H
#define BALL_VIEW_DIALOGS_MINIMIZATIONDIALOG_H

#include <BALL/VIEW/UIC/minimizationDialogData.h>

#include <BALL/FORMAT/INIFile.h>
#include <BALL/SYSTEM/path.h>

namespace BALL
{
	namespace VIEW
	{
		class AmberConfigurationDialog;
		class CharmmConfigurationDialog;

		/** Dialog for performing energy minimisations
				\ingroup ViewDialogs
		*/
		class BALL_EXPORT MinimizationDialog 
			: public MinimizationDialogData
		{
				Q_OBJECT

				public:

				/// Constructor
				MinimizationDialog(QWidget* parent = NULL, const char* name = NULL );

				/// Destructor
				virtual ~MinimizationDialog();

				/// 
				void writePreferences(INIFile& inifile) const;

				///
				void readPreferences(const INIFile& inifile);
				
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
				bool getUseConjugateGradient() const;
				
				///
				void setUseConjugateGradient(bool use_CG);
				
				/// Show an dialog for setting the options of the currently selected force field.
				void advancedOptions();
				
				/** Set the dialog for the AMBER Configuration.
						Called by MolecularStructure.
				*/
				void setAmberDialog(AmberConfigurationDialog* amber_dialog);
				
				/** Set the dialog for the CHARMM Configuration.
						Called by MolecularStructure.
				*/
				void setCharmmDialog(CharmmConfigurationDialog* charmm_dialog);
				
				/// Use the AMBER force field
				void useAmberFF();
				
				/// Use the CHARMM force field
				void useCharmmFF();
				
				/// Are we using the AMBER force field?
				bool getUseAmber();
				
				private:
				AmberConfigurationDialog* amber_dialog_;
				CharmmConfigurationDialog* charmm_dialog_;
		};
		
	} // namespace VIEW

} // namespace BALL

#endif
