// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: charmmConfigurationDialog.h,v 1.6.2.2 2006/05/31 19:43:19 amoll Exp $
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
			: public QDialog,
				public Ui_CharmmConfigurationDialogData,
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

			protected slots:

			virtual void resetOptions();
			
			virtual void browseParameterFiles();

			protected:

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
