// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: amberConfigurationDialog.h,v 1.1 2004/01/20 13:22:09 amoll Exp $
//

#ifndef BALL_VIEW_DIALOGS_AmberConfigurationDialog_H
#define BALL_VIEW_DIALOGS_AmberConfigurationDialog_H

#include <BALL/VIEW/UIC/AmberConfigurationDialogData.h>

#include <BALL/FORMAT/INIFile.h>
#include <BALL/SYSTEM/path.h>

namespace BALL
{
	namespace VIEW
	{
		/** Dialog for changing the options of the AMBER forcefield
				\ingroup ViewDialogs
		*/
		class AmberConfigurationDialog
			: public AmberConfigurationDialogData
		{
			Q_OBJECT
			
			public:

			/// Constructor
			AmberConfigurationDialog(QWidget* parent = NULL, const char* name = NULL);

			/// Destructor
			virtual ~AmberConfigurationDialog();
			
			///
			const String& getFilename() const;
			
			///
			void setFilename(const String& filename);

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
			void setOptions(float nonbonded_cutoff, float vdw_cutoff, float vdw_cuton, float electrostatic_cutoff, 
											float electrostatic_cuton, float scaling_electrostatic_1_4, float scaling_vdw_1_4, 
											bool use_dddc, bool assign_charges, bool assign_typenames, bool assign_types, 
											bool overwrite_charges, bool overwrite_typenames);

			///
			void writePreferences(INIFile& inifile) const
				throw();
				
			///
			void readPreferences(const INIFile& inifile)
				throw();

			protected:

			virtual void resetOptions();
			
			virtual void browseParameterFiles();

			private:

			bool 		use_dddc, assign_charges, assign_typenames, assign_types, overwrite_charges, overwrite_typenames;
			String 	ini;
			float 	nonbonded_cutoff, vdw_cutoff, vdw_cuton, electrostatic_cutoff, electrostatic_cuton,
							scaling_electrostatic_1_4, scaling_vdw_1_4;
		};
	}
}
#endif
