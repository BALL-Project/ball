// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_DIALOGS_AMBERMINIMIZATIONDIALOG_H
#define BALL_VIEW_DIALOGS_AMBERMINIMIZATIONDIALOG_H

#include <BALL/VIEW/UIC/amberMinimizationDialogData.h>

#include <BALL/FORMAT/INIFile.h>
#include <BALL/SYSTEM/path.h>

namespace BALL
{
	namespace VIEW
	{
		class AmberConfigurationDialog;

/** Dialog for performing energy minimisations
		\ingroup ViewDialogs
*/
class AmberMinimizationDialog 
	: public AmberMinimizationDialogData
{
    Q_OBJECT

		public:

		///
    AmberMinimizationDialog(QWidget* parent = NULL, const char* name = NULL );

		///
    virtual ~AmberMinimizationDialog();

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
		void setAmberDialog(AmberConfigurationDialog* amber_dialog);
	
		///
		void advancedOptions();
		
		private:
		AmberConfigurationDialog* amber_dialog_;
};

} } // namespaces
#endif
