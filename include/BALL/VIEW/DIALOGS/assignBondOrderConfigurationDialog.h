// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
//

#ifndef BALL_VIEW_DIALOGS_ASSIGNBONDORDERCONFIGURATIONDIALOG_H
#define BALL_VIEW_DIALOGS_ASSIGNBONDORDERCONFIGURATIONDIALOG_H

#include <BALL/VIEW/UIC/assignBondOrderConfigurationDialogData.h>

#ifndef BALL_VIEW_KERNEL_PREFERENCESENTRY
# include <BALL/VIEW/KERNEL/preferencesEntry.h>
#endif

#ifndef BALL_STRUCTURE_ASSIGNBONDORDERPROCESSOR_H
 # include <BALL/STRUCTURE/assignBondOrderProcessor.h>
#endif

namespace BALL
{
	class MMFF94;
	
	namespace VIEW
	{
		class MolecularStructure;

		/** Dialog for changing the options of the MMFF94 forcefield
				\ingroup ViewDialogs
		*/
		class BALL_VIEW_EXPORT AssignBondOrderConfigurationDialog
			: public QDialog,
				public Ui_AssignBondOrderConfigurationDialogData,
				public PreferencesEntry
		{
			friend class MolecularStructure;

			Q_OBJECT
			
			public:

			/// Constructor
			AssignBondOrderConfigurationDialog(QWidget* parent = NULL, const char* name = "AssignBondOrderConfigurationDialog");

			/// Destructor
			virtual ~AssignBondOrderConfigurationDialog();
			
			public slots:

			///
			virtual void resetOptions();
			
			///
			void accept();

			///
			void reject();

			//void readOptions(AssignBondOrderProcessor& assign_bond_proc) throw();
			/// apply the settings to a given MMFF94 force field
			//void applyTo(AssignBondOrderProcessor& assign_bond_proc)
			//	throw();

			protected slots:

			virtual void browseParameterFiles();

			protected:

			//void chooseSolventFile()
			//	throw();
		//	void setProc(AssignBondOrderProcessor& assign_bond_proc) throw();

			private:
			String getValue_(const QCheckBox* box) const;
			float getValue_(const QLineEdit* edit) const
				throw(Exception::InvalidFormat);

			//AssignBondOrderProcessor* assign_bond_proc_;
		};
	}
}
#endif
