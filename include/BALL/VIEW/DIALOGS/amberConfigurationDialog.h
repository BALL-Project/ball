// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: amberConfigurationDialog.h,v 1.6.8.2 2005/09/29 14:01:29 amoll Exp $
//

#ifndef BALL_VIEW_DIALOGS_AMBERCONFIGURATIONDIALOG_H
#define BALL_VIEW_DIALOGS_AMBERCONFIGURATIONDIALOG_H

#include <BALL/VIEW/UIC/amberConfigurationDialogData.h>

#ifndef BALL_VIEW_KERNEL_PREFERENCESENTRY
# include <BALL/VIEW/KERNEL/preferencesEntry.h>
#endif

namespace BALL
{
	class AmberFF;
	
	namespace VIEW
	{
		class MolecularStructure;

		/** Dialog for changing the options of the AMBER forcefield
				\ingroup ViewDialogs
		*/
		class BALL_VIEW_EXPORT AmberConfigurationDialog
			: public AmberConfigurationDialogData,
				public PreferencesEntry
		{
			friend class MolecularStructure;

			Q_OBJECT
			
			public:

			/// Constructor
			AmberConfigurationDialog(QWidget* parent = NULL, const char* name = NULL);

			/// Destructor
			virtual ~AmberConfigurationDialog();
			
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

			String getValue_(const QCheckBox* box) const;

			float getValue_(const QLineEdit* edit) const
				throw(Exception::InvalidFormat);

			AmberFF* amber_;
		};
	}
}
#endif
