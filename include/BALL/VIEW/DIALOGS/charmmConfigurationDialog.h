// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: charmmConfigurationDialog.h,v 1.2 2004/02/18 23:16:37 amoll Exp $
//

#ifndef BALL_VIEW_DIALOGS_CHARMMCONFIGURATIONDIALOG_H
#define BALL_VIEW_DIALOGS_CHARMMCONFIGURATIONDIALOG_H

#include <BALL/VIEW/UIC/charmmConfigurationDialogData.h>

#include <BALL/FORMAT/INIFile.h>
#include <BALL/SYSTEM/path.h>

namespace BALL
{
	class CharmmFF;
	
	namespace VIEW
	{
		class MolecularStructure;

		/** Dialog for changing the options of the CHARMM forcefield
				\ingroup ViewDialogs
		*/
		class CharmmConfigurationDialog
			: public CharmmConfigurationDialogData
		{
			friend class MolecularStructure;

			Q_OBJECT
			
			public:

			/// Constructor
			CharmmConfigurationDialog(QWidget* parent = NULL, const char* name = NULL);

			/// Destructor
			virtual ~CharmmConfigurationDialog();
			
			///
			const String& getFilename() const;
			
			///
			void setFilename(const String& filename);

			///
			bool getUseDistanceDependentDC() const;
			
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
			float getSolvationCutoff() const;

			///
			float getSolvationCuton() const;

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
			bool getUseEEF1() const;

			///
			void setOptions(float nonbonded_cutoff, float vdw_cutoff, float vdw_cuton, float electrostatic_cutoff, 
											float electrostatic_cuton, float solvation_cutoff, float solvation_cuton,
											float scaling_electrostatic_1_4, float scaling_vdw_1_4, 
											bool use_dddc, bool assign_charges, bool assign_typenames, bool assign_types, 
											bool overwrite_charges, bool overwrite_typenames, bool use_eef1);

			///
			void writePreferences(INIFile& inifile) const
				throw();
				
			///
			void fetchPreferences(const INIFile& inifile)
				throw();

			public slots:

			///
			void accept();

			///
			void reject();

			/// apply the settings to a given CHARMM force field
			void applyTo(CharmmFF& charmm)
				throw();

			protected:

			virtual void resetOptions();
			
			virtual void browseParameterFiles();

			void setCharmmFF(CharmmFF& charmm)
				throw();

			private:

			bool 		use_dddc_, 
							assign_charges_, 
							assign_typenames_, 
							assign_types_, 
							overwrite_charges_, 
							overwrite_typenames_, 
							use_eef1_;
							
			String 	ini_; 

			float 	nonbonded_cutoff_, 
							vdw_cutoff_, 
							vdw_cuton_, 
							electrostatic_cutoff_, 
							electrostatic_cuton_, 
							solvation_cutoff_, 
							solvation_cuton_,
							scaling_electrostatic_1_4_, 
							scaling_vdw_1_4_;

			CharmmFF* charmm_;
		};
	}
}
#endif
