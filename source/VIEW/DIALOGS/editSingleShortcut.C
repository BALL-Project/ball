// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/DIALOGS/editSingleShortcut.h>
#include <BALL/VIEW/KERNEL/shortcutRegistry.h>
#include <BALL/VIEW/KERNEL/common.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/SYSTEM/path.h>
#include <QtWidgets/QDialog>

namespace BALL
{
	namespace VIEW
	{

		EditSingleShortcut::EditSingleShortcut(QWidget* parent, const char* name, Qt::WindowFlags)
			: QDialog(parent),
				Ui_EditSingleShortcutData(),
				is_recording_(false),
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

			setShortcutText("Please insert your shortcut now.");
		}

		void EditSingleShortcut::accept()
		{
			stopRecording_();
			QDialog::accept();
		}

		void EditSingleShortcut::reject()
		{
			stopRecording_();
			QDialog::reject();
		}

		void EditSingleShortcut::setup(const QString& seq)
		{
			new_sequence_ = QKeySequence(seq);

			updateText_();

			setErrorText("");
			key_ = 0;
			modifiers_ = 0;

			if(!new_sequence_.isEmpty()) {
				customize_button->setChecked(true);
				none_button->setChecked(false);
			} else {
				customize_button->setChecked(false);
				none_button->setChecked(true);
			}

			changeMode_(new_sequence_ != QKeySequence());
		}

		void EditSingleShortcut::modeChanged_(bool toggled)
		{
			if(toggled) {
				changeMode_(customize_button->isChecked());
			}
		}

		void EditSingleShortcut::changeMode_(bool mode)
		{
			if(mode) {
				if(new_sequence_.isEmpty()) {
					setShortcutText("Please insert your shortcut.");
				}
				startRecording_();
			} else {
				key_ = 0;
				modifiers_ = 0;
				setShortcutText("None");
				stopRecording_();
				new_sequence_ = QKeySequence();
			}
		}

		void EditSingleShortcut::setShortcutText(QString new_keysequence)
		{
			new_shortcut_label->setText(new_keysequence);
		}

		void EditSingleShortcut::keyPressEvent(QKeyEvent* evt)
		{
			if(!is_recording_) {
				return;
			}

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
			if(!is_recording_) {
				return;
			}

			// Qt delivered a garbage keycode; Ignore it!
			if (evt->key() == -1) {
				return;
			}

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
			//"Escape" the ampersand character
			s.replace('&', QLatin1String("&&"));

			if (ShortcutRegistry::getInstance(0)->hasKey(new_sequence_))
			{
				setErrorText("Shortcut already in use.");
			}
			else
			{
				setErrorText("");
			}

			setShortcutText(s);
		}

		void EditSingleShortcut::setErrorText(QString error)
		{
			//make it nice and red
			error_label->setText("<span style=\" color:#ff0000;\">" + error + "</span>");
		}

		void EditSingleShortcut::startRecording_()
		{
			if(!is_recording_) {
				grabKeyboard();
				is_recording_ = true;
			}
		}

		void EditSingleShortcut::stopRecording_()
		{
			if(is_recording_) {
				releaseKeyboard();
				is_recording_ = false;
			}
		}

	} //namespace VIEW
} //namespace BALL

