// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_DIALOGS_ADVANCEDOPTIONSDIALOG_H
#define BALL_VIEW_DIALOGS_ADVANCEDOPTIONSDIALOG_H

#include <BALL/VIEW/UIC/advancedOptionsDialogData.h>

#include <BALL/FORMAT/INIFile.h>
#include <BALL/SYSTEM/path.h>

namespace BALL
{
	namespace VIEW
	{
		// Dialog for changing options in MolecularDynamicsDialog
		// and EnergyMinimizationDialog
		
		class advancedOptionsDialog
		: public advancedOptions
		{
			Q_OBJECT
			
			public:
			//constructor
			advancedOptionsDialog(QWidget* parent = NULL, const char* name = NULL);
			//destructor
			virtual ~advancedOptionsDialog();
			
			///
			const String& getFilename() const;
			
			///
			void setFilename(const String& filename);
/*//experimental
			void setNonbondedCutoff(float nonbonded_cutoff);
//experimental*/
			///
			bool getUseDistanceDependentDC() const;
			
			///
			float getNonbondedCutoff();

			///
			float getVdwCutoff();

			///
			float getVdwCuton();

			///
			float getElectrostaticCutoff();

			///
			float getElectrostaticCuton();

			///
			float getScalingElectrostatic_1_4();

			///
			float getScalingVdw_1_4();
			
			///
			bool getAssignCharges();
			
			///
			bool getAssignTypenames();
			
			///
			bool getAssignTypes();
			
			///
			bool getOverwriteCharges();
			
			///
			bool getOverwriteTypenames();
			
			///
			void setOptions(	float nonbonded_cutoff, float vdw_cutoff, float vdw_cuton, float electrostatic_cutoff, float electrostatic_cuton,
						float scaling_electrostatic_1_4, float scaling_vdw_1_4, bool use_dddc, bool assign_charges,
						bool assign_typenames, bool assign_types, bool overwrite_charges, bool overwrite_typenames);

			protected:

			virtual void resetOptions();
			
			virtual void browseParameterFiles();
		};
	}
}
#endif
