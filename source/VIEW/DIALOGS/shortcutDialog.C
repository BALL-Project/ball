// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/DIALOGS/shortcutDialog.h>
#include <BALL/VIEW/KERNEL/shortcutRegistry.h>
#include <BALL/VIEW/KERNEL/common.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/SYSTEM/path.h>

#include <QtGui/QFileDialog>

namespace BALL
{
	namespace VIEW
	{

		ShortcutDialog::ShortcutDialog(QWidget* parent, const char* name, Qt::WFlags)
			: QWidget(parent),
				Ui_ShortcutDialogData(),
				ModularWidget(name),
				PreferencesEntry()
		{
			// apply the dialogs layout
			setupUi(this);

			// define the dialogs section name in the INIFile
			setINIFileSectionName("BALLVIEWSHORTCUTS");

			setObjectName(name);

			// defines the dialogs Entry name
			setWidgetStackName(String(tr("Shortcuts")));

			registerWidgets_();

			//The shortcut registry has should be saved along with the other options for shortcuts
			registerObject_(ShortcutRegistry::getInstance(0));

			//The search should not be stored from session to session
			unregisterObject_(searchEdit);

			//TODO
			//registerWidgetForHelpSystem_(widget_stack->widget(0), "shortcuts.html#shortcuts");
			hide();

			connect(browse_export_button, SIGNAL(clicked()), this, SLOT(browseExportFile_()));
			connect(browse_import_button, SIGNAL(clicked()), this, SLOT(browseImportFile_()));
			connect(tableView, SIGNAL(shortcutChanged()), this, SLOT(shortcutChanged_()));
		}

		ShortcutDialog::~ShortcutDialog()
		{
		}

		void  ShortcutDialog::initializeWidget(MainControl&)
		{
			PreferencesEntry::restoreValues();
		}

		void ShortcutDialog::searchTextChanged(QString filter)
		{
			tableView->setFilter(filter);
		}

		void ShortcutDialog::browseImportFile_()
		{
			Path p;
			String filename = p.find("shortcuts_13.txt");
			QString s = QFileDialog::getOpenFileName(
			                0,
			                tr("Choose a file to import shortcuts from"),
			                filename.c_str(),
			                "Text files (*)");

			if (s.isEmpty())
			{
				// TODO this method should not occure if the dialog was canceled
				Log.warn()<< (String)tr("Could not import shortcuts from file ") << ascii(s) << std::endl;
				return;
			}
			else
			{
				loadShortcutsFromFile_(ascii(s));
			}
		}

		void ShortcutDialog:: browseExportFile_()
		{
			Path p;
			String filename = p.find("shortcuts_13.txt");

			QString s = QFileDialog::getSaveFileName(
			                0,
			                tr("Choose a File to export Shortcuts"),
			                filename.c_str(),
			                "Text files (*.*)");

			if (s.isEmpty()) return;

			bool ret = ShortcutRegistry::getInstance(0)->writeShortcutsToFile(ascii(s));
			if (!ret)
			{
				// TODO this method should not occure if the dialog was canceled
				Log.warn()<< (String)tr("Could not export shortcuts to file ") << ascii(s) << std::endl;
			}
		}

		void ShortcutDialog::loadPredefinedShortcuts_(QString entry)
		{
			Path p;
			String filename;

			if (entry == tr("Default Shortcuts (Version 1.3)"))
			{
				filename = p.find("BALLView/shortcuts_13.txt");
			} 
			else if (entry == tr("Legacy Shortcuts (Version 1.2)")) 
			{
				filename = p.find("BALLView/shortcuts_12.txt");
			} 
			else if (entry == tr("Custom")) 
			{
				return;
			}

			if (filename.isEmpty())
			{
				setStatusbarText((String)tr("Could not load legacy shortcuts."));
				return;
			}

			loadShortcutsFromFile_(filename);
		}

		void ShortcutDialog::shortcutChanged_()
		{
			predefined_combo_box->setCurrentIndex(2);
		}

		void ShortcutDialog::loadShortcutsFromFile_(const String& filename)
		{
			ShortcutRegistry* registry = ShortcutRegistry::getInstance(0);
			registry->clearKeySequences();
			bool ret = registry->readShortcutsFromFile(filename);
			if (!ret)
			{
				String description = (String)tr("Could not load shortcuts from file ") + filename + ".";
				setStatusbarText(description);
			}
			else
			{
				String description = (String)tr("Successfully loaded shortcuts from file ") + filename + ".";
				setStatusbarText(description);
			}
		}

	}//namespace VIEW
}//namespace BALL

