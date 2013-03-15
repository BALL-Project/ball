// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_DIALOGS_UNDOMANAGERDIALOG_H
#define BALL_VIEW_DIALOGS_UNDOMANAGERDIALOG_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#ifndef BALL_VIEW_KERNEL_MODULARWIDGET_H
# include <BALL/VIEW/KERNEL/modularWidget.h>
#endif

#ifndef BALL_VIEW_KERNEL_SNAPSHOT_MANAGER_INTERFACE_H
# include <BALL/VIEW/KERNEL/snapshotManagerInterface.h>
#endif

#include <QtWidgets/QDialog>

#include <BALL/VIEW/UIC/ui_undoManagerDialog.h>

namespace BALL
{
	namespace VIEW
	{
		/** Dialog for managing snapshots 
				\ingroup ViewDialogs
		*/
		class BALL_VIEW_EXPORT UndoManagerDialog
			: public QDialog,
				public Ui_UndoManagerDialogData,
				public ModularWidget
		{
			Q_OBJECT

			public:

				BALL_EMBEDDABLE(UndoManagerDialog,ModularWidget)

				/// Constructor
				UndoManagerDialog( QWidget* parent = 0, const char* name = "UndoManagerDialog", Qt::WindowFlags fl = 0 );

				/// Destructor
				~UndoManagerDialog() {}

				///
				virtual void initializeWidget(MainControl& main_control);

				///
				virtual void checkMenu(MainControl& main_control);

				/// Overrides message in order to intercept system changed events
				virtual void onNotify(Message* message);

			public slots:

				/// Show and raise dialog
				void show();

				///
				void close_pressed();

				///
				void resetTo_pressed();

				///
				void takeSnapshot_pressed();

			protected:

				QAction* action_;
				SnapshotManagerInterface snapshot_interface_;

				bool first_run;

		};

	}
} // namespaces

#endif
