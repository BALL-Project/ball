// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:

#include <BALL/VIEW/DIALOGS/undoManagerDialog.h>
#include <BALL/VIEW/KERNEL/common.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/COMMON/logStream.h>

#include <QtWidgets/QLineEdit>
#include <QtWidgets/QRadioButton>
#include <QtCore/QDateTime>

namespace BALL
{
	namespace VIEW
	{
		UndoManagerDialog::UndoManagerDialog( QWidget* parent, const char* name, Qt::WindowFlags fl )
			: QDialog(parent, fl),
			  Ui_UndoManagerDialogData(),
				ModularWidget(name),
				action_(0),
				first_run(true)
		{
			#ifdef BALL_VIEW_DEBUG
				Log.error() << "new undoManagerDialog " << this << std::endl;
			#endif

			setupUi(this);
			setObjectName(name);

			// register the widget with the MainControl
			ModularWidget::registerWidget(this);
			hide();

			// signals and slots connections
			connect( close_button, SIGNAL( pressed() ), this, SLOT( close_pressed() ) );
			connect( add_snapshot_button, SIGNAL( pressed() ), this, SLOT( takeSnapshot_pressed() ) );
			connect( reset_to_button, SIGNAL( pressed() ), this, SLOT( resetTo_pressed() ) );
			close_button->setEnabled(true);
		}

		void UndoManagerDialog::initializeWidget(MainControl&)
		{
			String description = "Shortcut|Edit|Undo_Manager";
			action_ = insertMenuEntry(MainControl::EDIT, tr("Open Undo Manager"), this, 
			                          SLOT(show()), description, QKeySequence(),
																tr("Start the undo manager dialog."), UIOperationMode::MODE_ADVANCED);

			// TODO icon
			// setIcon("actions/undo-manager", true);
		}

		void UndoManagerDialog::checkMenu(MainControl& main_control)
		{
			bool busy = main_control.compositesAreLocked();
			if (action_) action_->setEnabled(!busy);
		}

		void UndoManagerDialog::onNotify(Message* message)
		{
			CompositeMessage* msg = dynamic_cast<CompositeMessage*>(message);

			if(msg && (msg->getType() != CompositeMessage::CENTER_CAMERA)
						 && (msg->getType() != CompositeMessage::CHANGED_COMPOSITE)
			       && (msg->getType() != CompositeMessage::SELECTED_COMPOSITE)
						 && (msg->getType() != CompositeMessage::DESELECTED_COMPOSITE))
			{
				if(!first_run) {
					snapshot_interface_.setup();
					reset_to_button->setEnabled(false);
					snapshot_list->clear();
				}
			}
		}

		void UndoManagerDialog::show()
		{
			QDialog::show();
			raise();
		}

		void UndoManagerDialog::close_pressed()
		{
			hide();
		}

	  void UndoManagerDialog::resetTo_pressed()
		{
			if(snapshot_list->currentRow() >= 0)
			{
				snapshot_interface_.applySnapshot(snapshot_list->currentRow());
			}
		}

	  void UndoManagerDialog::takeSnapshot_pressed()
		{
			if(first_run)
			{
				snapshot_interface_.setup();
				first_run = false;
			}

			QString label = QDateTime::currentDateTime().toLocalTime().toString("hh:mm:ss");
			snapshot_interface_.takeSnapshot(String(label));
			snapshot_list->addItem(label);
			reset_to_button->setEnabled(true);
		}

	} // namespace VIEW
} // namespace BALL
