// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/DIALOGS/editSingleShortcut.h>
#include <BALL/VIEW/KERNEL/shortcutRegistry.h>
#include <BALL/VIEW/KERNEL/common.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/SYSTEM/path.h>
#include <QtGui/QDialog>

namespace BALL
{
	namespace VIEW
	{

		EditSingleShortcut::EditSingleShortcut(QWidget* parent, const char* name, Qt::WFlags)
			: QDialog(parent),
				Ui_EditSingleShortcutData(),
				custom_shortcut_mode_(true),
			  modifiers_(0),
				key_(0)
		{
			// apply the dialogs layout
			setupUi(this);

			setObjectName(name);
			setAutoFillBackground(true);


			// signals and slots connections
			new_shortcut_label->setText("Please type key sequence!");

			connect( buttonBox, SIGNAL( accepted() ), this, SLOT( accept() ) );
			connect( buttonBox, SIGNAL( rejected() ), this, SLOT( reject() ) );

			connect(customize_button, SIGNAL(toggled(bool)), this, SLOT(modeChanged_(bool)));
			connect(none_button,      SIGNAL(toggled(bool)), this, SLOT(modeChanged_(bool)));

			new_shortcut_label->setText("Please type key sequence!");
			setShortcutText("Please insert your shortcut now.");
			grabKeyboard();
		}

		void EditSingleShortcut::modeChanged_(bool /*toggled*/)
		{
			custom_shortcut_mode_ = customize_button->isChecked();
			if (custom_shortcut_mode_)
				new_shortcut_label->setText("Please insert your shortcut.");
			else
				new_shortcut_label->setText("Set shortcut to None.");
		}

		void EditSingleShortcut::setShortcutText(QString new_keysequence)
		{
			new_shortcut_label->setText(new_keysequence);
		}

		void EditSingleShortcut::keyPressEvent(QKeyEvent* evt)
		{
			// Qt delivered a garbage keycode; Ignore it!
			if (evt->key() == -1)
			{
				return;
			}

			// If AltGr is pressed there is a problem with unicode
			// So just bail out...

			evt->accept();

			switch (evt->key())
			{
				case Qt::Key_AltGr: //or else we get unicode salad
					return;
				case Qt::Key_Shift:
				case Qt::Key_Control:
				case Qt::Key_Alt:
				case Qt::Key_Meta:
				case Qt::Key_Menu: //unused (yes, but why?)
					break;
				default:
					key_ = evt->key();
			}

			if ((key_ == 0) || (key_ == Qt::Key_unknown))
			{
				// Filter the interesting modifiers
				modifiers_ = evt->modifiers() & (Qt::ControlModifier | Qt::AltModifier |
																				 Qt::ShiftModifier | Qt::MetaModifier);
			}

			new_sequence_ = QKeySequence(key_ | modifiers_);

			updateText_();
		}

		void EditSingleShortcut::keyReleaseEvent(QKeyEvent* evt)
		{
			// Qt delivered a garbage keycode; Ignore it!
			if (evt->key() == -1) {
				return;
			}

			bool released_key = false;

			switch (evt->key()) {
				case Qt::Key_AltGr: //or else we get unicode salad
					return;
				case Qt::Key_Shift:
				case Qt::Key_Control:
				case Qt::Key_Alt:
				case Qt::Key_Meta:
				case Qt::Key_Menu: //unused (yes, but why?)
					break;
				default:
					released_key = true;
					key_ = evt->key();
			}

			if ((key_ == 0) || (key_ == Qt::Key_unknown))
			{
				// Filter the interesting modifiers
				modifiers_ = evt->modifiers() & (Qt::ControlModifier | Qt::AltModifier |
																				 Qt::ShiftModifier | Qt::MetaModifier);
			}

			new_sequence_ = QKeySequence(key_ | modifiers_);
			evt->accept();

			updateText_();
		}

		void EditSingleShortcut::updateText_()
		{
			QString s = new_sequence_.toString(QKeySequence::NativeText);
			s.replace('&', QLatin1String("&&"));

			setShortcutText(s);
		}

		void EditSingleShortcut::setErrorText(QString error)
		{
			error_label->setText(error);
		}

	}//namespace VIEW
}//namespace BALL
