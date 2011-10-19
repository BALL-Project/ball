// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
//

#ifndef BALL_VIEW_DIALOGS_ASSIGNBONDORDERCONFIGURATIONDIALOG_H
#define BALL_VIEW_DIALOGS_ASSIGNBONDORDERCONFIGURATIONDIALOG_H

#ifndef BALL_VIEW_KERNEL_PREFERENCESENTRY
# include <BALL/VIEW/KERNEL/preferencesEntry.h>
#endif

#ifndef BALL_STRUCTURE_ASSIGNBONDORDERPROCESSOR_H
# include <BALL/STRUCTURE/assignBondOrderProcessor.h>
#endif

#ifndef BALL_VIEW_KERNEL_MODULARWIDGET_H
# include <BALL/VIEW/KERNEL/modularWidget.h>
#endif

#include <BALL/VIEW/UIC/ui_assignBondOrderConfigurationDialog.h>

namespace BALL
{
	class MMFF94;

	namespace VIEW
	{
		class MolecularStructure;
		class MainControl;

		/** Dialog for changing the options of the assignBondOrderProcessor
				\ingroup ViewDialogs
		*/
		class BALL_VIEW_EXPORT AssignBondOrderConfigurationDialog
			: public QDialog,
				public Ui_AssignBondOrderConfigurationDialogData,
				public ModularWidget,
				public PreferencesEntry
		{
			friend class MolecularStructure;

			Q_OBJECT

			public:

			BALL_EMBEDDABLE(AssignBondOrderConfigurationDialog,ModularWidget)

			/// Constructor
			AssignBondOrderConfigurationDialog(QWidget* parent = NULL, const char* name = "AssignBondOrderConfigurationDialog");

			/// Destructor
			virtual ~AssignBondOrderConfigurationDialog();

			///
			virtual void initializeWidget(MainControl& main_control);

			public slots:

			///
			virtual void resetOptions();

			///
			void accept();

			///
			void reject();

			protected slots:

				virtual void browseParameterFiles_();
				virtual void balanceParameterChanged_();

				virtual void validateBOBoxes_();
				virtual void validateStrategies_();

				virtual void dialogButtonClicked_(QAbstractButton* button);

			protected:

			private:

				String getValue_(const QCheckBox* box) const;

				float getValue_(const QLineEdit* edit) const
					throw(Exception::InvalidFormat);
		};
	}
}
#endif
