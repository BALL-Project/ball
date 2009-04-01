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
			:	QWidget(parent),
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
			setWidgetStackName("Shortcuts");

			// for preferences	
			registerWidgets_();

			// register the widget with the MainControl  for actions, icons...
			//ModularWidget::registerWidget(this);
			
			//TODO
			//registerWidgetForHelpSystem_(widget_stack->widget(0), "shortcuts.html#shortcuts");
			hide();
		
			connect( browse_export_button, SIGNAL( clicked() ), this, SLOT( browseExportFile_() ) );	
			connect( browse_import_button, SIGNAL( clicked() ), this, SLOT( browseImportFile_() ) );	

			connect(use_legacy_shortcuts, SIGNAL(stateChanged(int)), this, SLOT(loadLegacyShortcuts_()));

			validateShortcuts_();
		}

		ShortcutDialog::~ShortcutDialog()
		{
		}

		void  ShortcutDialog::initializeWidget(MainControl&)
		{
			std::cout << "TODO: read last used shortcuts from preferences" << std::endl;
			PreferencesEntry::restoreValues();
		}
		
		void ShortcutDialog:: browseImportFile_()
		{
			Path p;
			String filename = p.find("shortcuts_13.txt");
			QString s = QFileDialog::getOpenFileName(
											0,
											"Choose a File to import Shortcuts",
											//getMainControl()->getWorkingDir().c_str(),
											filename.c_str(),
											"Text files (*.*)");

			if (s.isEmpty())
			{	
				// TODO this method should not occure if the dialog was canceled
				Log.warn()<< "Could not import shortcuts from file " << ascii(s) << std::endl; 
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
											"Choose a File to export Shortcuts",
											//getMainControl()->getWorkingDir().c_str(),
											filename.c_str(),
											"Text files (*.*)");

			if (s.isEmpty())	return;
			
			bool ret = ShortcutRegistry::getInstance(0)->writeShortcutsToFile(ascii(s));
			if (!ret)
			{	
				// TODO this method should not occure if the dialog was canceled
				Log.warn()<< "Could not export shortcuts to file " << ascii(s) << std::endl; 
			}

		}

		void ShortcutDialog::validateShortcuts_()
		{
				std::cout << " TODO ShortcutDialog::validateShortcuts_()" << std::endl;
		}

		void ShortcutDialog::loadLegacyShortcuts_()
		{		
			bool  load_legacy_shortcuts = use_legacy_shortcuts->isChecked();

			if (load_legacy_shortcuts)
			{
				Path p;
				String filename = p.find("BALLView/shortcuts_12.txt");
				if (filename.isEmpty())	
				{
					setStatusbarText("Could not load legacy shortcuts.");
					return;
				}
				loadShortcutsFromFile_(filename);
			}
		}

		void ShortcutDialog::loadShortcutsFromFile_(const String& filename)
		{
			ShortcutRegistry* registry = ShortcutRegistry::getInstance(0);
			registry->clearKeySequences(); 
			bool ret = registry->readShortcutsFromFile(filename);
			if (!ret)
			{
				String description = 	"Could not load shortcuts from file " +  filename + ".";
				setStatusbarText(description);
			}
			else
			{
				String description = 	"Successfully loaded shortcuts from file " +  filename + ".";
				setStatusbarText(description);
			}
		}		
	}//namespace VIEW
}//namespace BALL
