// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: amberConfigurationDialog.h,v 1.4 2004/02/26 08:41:31 anhi Exp $
//

#ifndef BALL_VIEW_DIALOGS_AMBERCONFIGURATIONDIALOG_H
#define BALL_VIEW_DIALOGS_AMBERCONFIGURATIONDIALOG_H

#include <BALL/VIEW/UIC/amberConfigurationDialogData.h>

#include <BALL/FORMAT/INIFile.h>
#include <BALL/SYSTEM/path.h>

namespace BALL
{
	class AmberFF;
	
	namespace VIEW
	{
		class MolecularStructure;

		/** Dialog for changing the options of the AMBER forcefield
				\ingroup ViewDialogs
		*/
		class BALL_EXPORT AmberConfigurationDialog
			: public AmberConfigurationDialogData
		{
			friend class MolecularStructure;

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
			void setOptions(float nonbonded_cutoff, float vdw_cutoff, float vdw_cuton, float electrostatic_cutoff, 
											float electrostatic_cuton, float scaling_electrostatic_1_4, float scaling_vdw_1_4, 
											bool use_dddc, bool assign_charges, bool assign_typenames, bool assign_types, 
											bool overwrite_charges, bool overwrite_typenames);

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

			/// apply the settings to a given AMBER force field
			void applyTo(AmberFF& amber)
				throw();

			protected:

			virtual void resetOptions();
			
			virtual void browseParameterFiles();

			void setAmberFF(AmberFF& amber)
				throw();

			private:

			bool 		use_dddc_, assign_charges_, assign_typenames_, assign_types_, overwrite_charges_, overwrite_typenames_;
			String 	ini_;
			float 	nonbonded_cutoff_, vdw_cutoff_, vdw_cuton_, electrostatic_cutoff_, electrostatic_cuton_,
							scaling_electrostatic_1_4_, scaling_vdw_1_4_;

			AmberFF* amber_;
		};
	}
}
#endif
