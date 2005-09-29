// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: charmmConfigurationDialog.h,v 1.5.8.2 2005/09/29 14:01:29 amoll Exp $
//

#ifndef BALL_VIEW_DIALOGS_CHARMMCONFIGURATIONDIALOG_H
#define BALL_VIEW_DIALOGS_CHARMMCONFIGURATIONDIALOG_H

#include <BALL/VIEW/UIC/charmmConfigurationDialogData.h>

#ifndef BALL_VIEW_KERNEL_PREFERENCESENTRY
# include <BALL/VIEW/KERNEL/preferencesEntry.h>
#endif

namespace BALL
{
	class CharmmFF;
	
	namespace VIEW
	{
		class MolecularStructure;

		/** Dialog for changing the options of the CHARMM forcefield
				\ingroup ViewDialogs
		*/
		class BALL_VIEW_EXPORT CharmmConfigurationDialog
			: public CharmmConfigurationDialogData,
				public PreferencesEntry
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
			
			public slots:

			///
			void accept();

			///
			void reject();

			/// apply the settings to a given CHARMM force field
			void applyTo(CharmmFF& charmm)
				throw();
			
			///
			void chooseSolventFile()
				throw();

			//_
			void periodicBoundaryClicked()
				throw();

			protected:

			virtual void resetOptions();
			
			virtual void browseParameterFiles();

			void setCharmmFF(CharmmFF& charmm)
				throw();

			private:

			String getValue_(const QCheckBox* box) const;

			float getValue_(const QLineEdit* edit) const
				throw(Exception::InvalidFormat);

			CharmmFF* charmm_;
		};
	}
}
#endif
