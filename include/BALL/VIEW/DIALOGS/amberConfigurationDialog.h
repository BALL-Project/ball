// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: amberConfigurationDialog.h,v 1.6 2004/04/23 13:08:13 amoll Exp $
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

			//_
			void periodicBoundaryClicked()
				throw();

			protected:

			virtual void resetOptions();
			
			virtual void browseParameterFiles();

			void setAmberFF(AmberFF& amber)
				throw();

			void chooseSolventFile()
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
