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
class MinimizationDialog 
	: public MinimizationDialogData
{
    Q_OBJECT

		public:

		///
    MinimizationDialog(QWidget* parent = NULL, const char* name = NULL );

		///
    virtual ~MinimizationDialog();

		///
		void writePreferences(INIFile& inifile) const;

		///
		void readPreferences(const INIFile& inifile);
		
		///
		Size getMaxIterations() const;

		///
		void setMaxIterations(Size n);
	
		///
		Size getRefresh() const;
		
		///
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
		
		///
		void advancedOptions();
		
		///
		void setAmberDialog(AmberConfigurationDialog* amber_dialog);
		
		///
		void setCharmmDialog(CharmmConfigurationDialog* charmm_dialog);
		
		///
		void setForceField(bool amber);
		
		///
		void useAmberFF();
		
		///
		void useCharmmFF();
		
		///
		bool getUseAmber();
		
		private:
		AmberConfigurationDialog* amber_dialog_;
		CharmmConfigurationDialog* charmm_dialog_;
};

} } // namespaces
#endif
