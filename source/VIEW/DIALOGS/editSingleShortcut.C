// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/DIALOGS/editSingleShortcut.h>
#include <BALL/VIEW/KERNEL/shortcutRegistry.h>
#include <BALL/VIEW/KERNEL/common.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/SYSTEM/path.h>
#include <QtGui/QWidget>

namespace BALL
{
	namespace VIEW
	{

		EditSingleShortcut::EditSingleShortcut(QWidget* parent, const char* name, Qt::WFlags)
			:	QWidget(parent),
				Ui_EditSingleShortcutData(),
				ModularWidget(name),
				PreferencesEntry(),
				custom_shortcut_mode_(true),
				edited_(false)
		{
			// apply the dialogs layout
			setupUi(this);

			setObjectName(name);
			
			// for preferences	
			registerWidgets_();

			// register the widget with the MainControl  for actions, icons...
			hide();
		
			// signals and slots connections
			connect( buttonBox, SIGNAL( accepted() ), this, SLOT( accepted() ) );
			connect( buttonBox, SIGNAL( rejected() ), this, SLOT( rejected() ) );
			
			connect( customize_button, SIGNAL( toggled(bool) ), this, SLOT( newMode_(bool) ) );
			connect( none_button, SIGNAL( toggled(bool) ), this, SLOT( newMode_(bool) ) );
			
			new_shortcut_label->setText("Please type key sequence!");	
		}

		EditSingleShortcut::~EditSingleShortcut()
		{
		}

		void  EditSingleShortcut::initializeWidget(MainControl&)
		{
			PreferencesEntry::restoreValues();
		}
		
		void EditSingleShortcut::rejected()
		{
			edited_ = false;
			hide();
			PreferencesEntry::restoreValues();
		}

		void EditSingleShortcut::accepted()
		{
			edited_ = true;
			hide();
		}
		
		void EditSingleShortcut::modeChanged_(bool /*toggled*/)
		{
			custom_shortcut_mode_ = customize_button->isChecked();
			if (custom_shortcut_mode_)
				new_shortcut_label->setText("Please insert your shortcut now.");
			else
				new_shortcut_label->setText("Set shortcut to None.");
		}

		void EditSingleShortcut::setShortcutText(QString new_keysequence)
		{
			new_shortcut_label->setText(new_keysequence);	
		}

	}//namespace VIEW
}//namespace BALL
