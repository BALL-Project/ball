// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_DIALOGS_MOLECULARDYNAMICSDIALOG_H
#define BALL_VIEW_DIALOGS_MOLECULARDYNAMICSDIALOG_H

#include <BALL/VIEW/UIC/molecularDynamicsDialogData.h>

#include <BALL/FORMAT/INIFile.h>
#include <BALL/SYSTEM/path.h>

namespace BALL
{
	namespace VIEW
	{
		class AmberConfigurationDialog;

/** Dialog for performing MD simulations
		\ingroup ViewDialogs
*/
class MolecularDynamicsDialog 
	: public MolecularDynamicsDialogData
{
    Q_OBJECT

		public:

		///
    MolecularDynamicsDialog(QWidget* parent = NULL, const char* name = NULL );

		///
    virtual ~MolecularDynamicsDialog();

		///
		void writePreferences(INIFile& inifile) const;

		///
		void readPreferences(const INIFile& inifile);

		///
		float getSimulationTime() const;

		///
		Size getNumberOfSteps() const;

		///
		void setNumberOfSteps(Size steps);
	
		///
		void setTimeStep(float time);

		///
		float getTimeStep() const;

		///
		bool useMicroCanonical() const;
		
		///
		void setMicroCanonical(bool state);

		///
		void setTemperature(float temperature);

		///
		float getTemperature() const;

		///
		bool saveImages() const;

		/// 
		String getDCDFile() const;

		///
		Size getStepsBetweenRefreshs() const;
		
		///
		void advancedOptions();

		///
		void setAmberDialog(AmberConfigurationDialog* amber_dialog);
	
		protected slots:

		virtual void enableDCDFileSelected();

		void timeChanged();

		private:
		AmberConfigurationDialog* amber_dialog_;
};

} } // namespaces
#endif
