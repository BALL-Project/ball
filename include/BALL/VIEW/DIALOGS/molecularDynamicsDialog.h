// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_DIALOGS_MOLECULARDYNAMICSDIALOG_H
#define BALL_VIEW_DIALOGS_MOLECULARDYNAMICSDIALOG_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#ifndef BALL_VIEW_KERNEL_PREFERENCESENTRY
# include <BALL/VIEW/KERNEL/preferencesEntry.h>
#endif

#include <BALL/VIEW/UIC/ui_molecularDynamicsDialog.h>

namespace BALL
{
	namespace VIEW
	{
		class AmberConfigurationDialog;
		class CharmmConfigurationDialog;
		class MMFF94ConfigurationDialog;

/** Dialog for performing MD simulations
		\ingroup ViewDialogs
*/
class BALL_VIEW_EXPORT MolecularDynamicsDialog 
	: public QDialog,
		public Ui_MolecularDynamicsDialogData,
		public PreferencesEntry
{
    Q_OBJECT

		public:

		///
    MolecularDynamicsDialog(QWidget* parent = NULL, const char* name = "MolecularDynamicsDialog" );

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
		QString getDCDFile() const;

		///
		void setDCDFile(const String& name);

		///
		Size getStepsBetweenRefreshs() const;
		
		///
		void setAmberDialog(AmberConfigurationDialog* amber_dialog);
		
		///
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

		protected slots:

		virtual void enableDCDFileSelected();
		
		///
		void advancedOptions();
	
		///
		void chooseDCDFile();
	
		void timeChanged();

		private:
		AmberConfigurationDialog* amber_dialog_;
		CharmmConfigurationDialog* charmm_dialog_;
		MMFF94ConfigurationDialog* mmff_dialog_;
};

} } // namespaces
#endif
