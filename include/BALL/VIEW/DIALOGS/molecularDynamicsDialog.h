// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_DIALOGS_MOLECULARDYNAMICSDIALOG_H
#define BALL_VIEW_DIALOGS_MOLECULARDYNAMICSDIALOG_H

#include <BALL/VIEW/UIC/molecularDynamicsDialogData.h>

#ifndef BALL_VIEW_KERNEL_PREFERENCESENTRY
# include <BALL/VIEW/KERNEL/preferencesEntry.h>
#endif

namespace BALL
{
	namespace VIEW
	{
		class AmberConfigurationDialog;
		class CharmmConfigurationDialog;

/** Dialog for performing MD simulations
		\ingroup ViewDialogs
*/
class BALL_VIEW_EXPORT MolecularDynamicsDialog 
	: public MolecularDynamicsDialogData,
		public PreferencesEntry
{
    Q_OBJECT

		public:

		///
    MolecularDynamicsDialog(QWidget* parent = NULL, const char* name = NULL );

		///
    virtual ~MolecularDynamicsDialog();

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
		String getDCDFile() const;

		///
		Size getStepsBetweenRefreshs() const;
		
		///
		void advancedOptions();

		///
		void setAmberDialog(AmberConfigurationDialog* amber_dialog);
		
		///
		void setCharmmDialog(CharmmConfigurationDialog* charmm_dialog);
		
		///
		void useAmberFF();
		
		///
		void useCharmmFF();
		
		///
		bool getUseAmber();

		///
		void chooseDCDFile();
	
		protected slots:

		virtual void enableDCDFileSelected();

		void timeChanged();

		private:
		AmberConfigurationDialog* amber_dialog_;
		CharmmConfigurationDialog* charmm_dialog_;
};

} } // namespaces
#endif
