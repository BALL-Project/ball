// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_DIALOGS_CHARMMCONFIGURATIONDIALOG_H
#define BALL_VIEW_DIALOGS_CHARMMCONFIGURATIONDIALOG_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#ifndef BALL_VIEW_KERNEL_PREFERENCESENTRY
# include <BALL/VIEW/KERNEL/preferencesEntry.h>
#endif

#include <BALL/VIEW/UIC/ui_charmmConfigurationDialog.h>

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
			CharmmConfigurationDialog(QWidget* parent = NULL, const char* name = "CharmmConfiguration");

			/// Destructor
			virtual ~CharmmConfigurationDialog();
			
			///
			const String& getFilename() const;
			
			public slots:

			virtual void resetOptions();
			
			///
			void accept();

			///
			void reject();

			/// apply the settings to a given CHARMM force field
			void applyTo(CharmmFF& charmm);
			
			///
			void chooseSolventFile();

			//_
			void periodicBoundaryClicked();

			protected slots:

			virtual void browseParameterFiles();

			protected:

			void setCharmmFF(CharmmFF& charmm);

			private:

			String getValue_(const QCheckBox* box) const;

			float getValue_(const QLineEdit* edit) const
				throw(Exception::InvalidFormat);

			CharmmFF* charmm_;
		};
	}
}
#endif
