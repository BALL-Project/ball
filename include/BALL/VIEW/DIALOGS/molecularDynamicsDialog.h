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

/** Dialog for performing MD simulations
		\ingroup ViewDialogs
*/
class MolecularDynamicsDialog 
	: public MolecularDynamicsDialogData
{
    Q_OBJECT

		bool use_dddc, assign_charges, assign_typenames, assign_types, overwrite_charges, overwrite_typenames;
		String ini;
		float 	nonbonded_cutoff, vdw_cutoff, vdw_cuton, electrostatic_cutoff, electrostatic_cuton,
			scaling_electrostatic_1_4, scaling_vdw_1_4;
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
		float getNonbondedCutoff() const;
		
		///
		float getVdwCutoff() const;
		
		///
		float getVdwCuton() const;
		
		///
		float getElectrostaticCutoff() const;
		
		///
		float getElectrostaticCuton() const;
		
		///
		float getScalingElectrostatic_1_4() const;
		
		///
		float getScalingVdw_1_4() const;
		
		///
		bool getAssignCharges() const;

		///
		bool getAssignTypenames() const;

		///
		bool getAssignTypes() const;

		///
		bool getOverwriteCharges() const;

		///
		bool getOverwriteTypenames() const;
		
		///
		const String& getFilename() const;
		
		///
		void setFilename(const String& filename);

		///
		bool getUseDistanceDependentDC() const;
		
		///
		void setUseDistanceDependentDC(bool usedddc);

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
		
		void advancedOptions();

		protected slots:
		virtual void browseParameterFiles();

		virtual void enableDCDFileSelected();

		void timeChanged();
};

} } // namespaces
#endif
