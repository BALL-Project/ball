// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#include <BALL/VIEW/DIALOGS/pythonSettings.h>
#include <BALL/VIEW/KERNEL/mainControl.h>

#include <qlineedit.h>
#include <qfiledialog.h>
#include <qpushbutton.h>
#include <qwidgetstack.h>

namespace BALL
{
	namespace VIEW
	{

HotkeyTable::HotkeyTable(QWidget* parent,  const char*)
	throw()
	: QTable(parent)
{
  horizontalHeader()->setLabel(0, "Modifier");
  horizontalHeader()->setLabel(1, "Key");
  horizontalHeader()->setLabel(2, "Command");

  setNumRows(0);
  setNumCols(3);
	setShowGrid(true);
	
	setColumnWidth(0, 70);
	setColumnWidth(1, 60);
	setColumnWidth(2, 350);
	setGeometry(5,5, 534, 320);

	setSelectionMode(QTable::SingleRow);

	QStringList labels;
	labels << "Modifier" << "Key" << "Command";
	setColumnLabels(labels);

	setName("PythonHotkeys");

	modifier_ << "" << "Shift";// << "Alt";

	for (Position p = 1; p < 13; p++)
	{
		keys_ << (String("F") + String(p)).c_str();
	}
	
	// F2 -> runScriptAgain()
	appendHotkey(NONE, 2, "runScriptAgain()");

	// F3 -> hideAllRepresentations()
	appendHotkey(NONE, 3, "hideAllRepresentations()");

	// F4 -> removeWater()
	appendHotkey(NONE, 4, "removeWater()");

	// F5 -> printAtomTypesForHighlighted()
	appendHotkey(NONE, 5, "printAtomTypesForHighlighted()");

	// F5 -> printAtomTypesForLigands()
	appendHotkey(SHIFT, 5, "printAtomTypesForLigands()");

	// F6 -> showCartoonAndLigand()
	appendHotkey(NONE, 6, "highlightLigand()");

	// S-F6 -> showCartoonAndLigand()
	appendHotkey(SHIFT, 6, "showCartoonAndLigand()");

	// F7 -> addOptimizedHydrogens()
	appendHotkey(NONE, 7, "addOptimizedHydrogens()");
	
	// S-F7 -> relaxStructure()
	appendHotkey(SHIFT, 7, "relaxStructure()");
	
	// F8 -> quickSave()
	appendHotkey(NONE, 8, "quickSave()");
	
	// S-F8 -> quickLoad()
	appendHotkey(SHIFT, 8, "quickLoad()");
	
	// F12 -> clearRepresentations()
	appendHotkey(NONE, 12, "clearRepresentations()");
	
	// S-F12 -> clearAll()
	appendHotkey(SHIFT, 12, "clearAll()");
}

void HotkeyTable::appendHotkey(Modifier mod, Position F_key, const String& command)
{
	addEmptyRow();
	((QComboTableItem*)item(numRows() - 1, 0))->setCurrentItem((Position)mod);
	((QComboTableItem*)item(numRows() - 1, 1))->setCurrentItem(F_key - 1);
	item(numRows() - 1, 2)->setText(command.c_str());
}

List<Hotkey> HotkeyTable::getContent() const
	throw()
{
	List<Hotkey> result;
	for (Index pos = 0; pos < numRows(); pos++)
	{
		if (item(pos, 2)->text().isEmpty() ||
				!RTTI::isKindOf<QComboTableItem>(*item(pos, 0))) 
		{
			Log.error() << "Problem reading content of PythonHotkeys" << std::endl;
			continue;
		}

		Hotkey hotkey;

		Index ci = ((QComboTableItem*)item(pos, 0))->currentItem();
		switch(ci)
		{
      case 0:
        hotkey.button_state = Qt::NoButton;
        break;

      case 1:
        hotkey.button_state = Qt::ShiftButton;
        break;

      case 2:
        hotkey.button_state = Qt::ControlButton;
        break;

      default:
        Log.error() << "Problem reading content of PythonHotkeys" << std::endl;
    }

    ci = ((QComboTableItem*) item(pos, 1))->currentItem();
    hotkey.key = (Qt::Key) (Qt::Key_F1 + ci);

    hotkey.action = item(pos, 2)->text().ascii();

    result.push_back(hotkey);
  }

  return result;
}


void HotkeyTable::setContent(const List<Hotkey>& hotkeys)
	throw()
{
  setNumRows(hotkeys.size());
	setColumnWidth(2, 230);

	List<Hotkey>::ConstIterator it = hotkeys.begin();
	Position p = 0;
	for (; it != hotkeys.end(); it++)
	{
		QComboTableItem * item = new QComboTableItem(this, modifier_, FALSE );
		switch ((Position)(*it).button_state)
		{
			case (Position)Qt::NoButton:
				item->setCurrentItem(0);
				break;

			case (Position)Qt::ShiftButton:
				item->setCurrentItem(1);
				break;

			case (Position)Qt::ControlButton:
				item->setCurrentItem(2);
				break;

			default:
				Log.error() << "Invalid button state for Hotkey" << std::endl;
		}
    setItem(p, 0,  item) ;

		QComboTableItem * item2 = new QComboTableItem(this, keys_, FALSE );
		item2->setCurrentItem((*it).key - Qt::Key_F1);
    setItem(p, 1,  item2) ;

    setText(p, 2,  (*it).action.c_str());

		p++;
	}
}

void HotkeyTable::addEmptyRow()
{
	setNumRows(numRows() + 1);
	QComboTableItem * item = new QComboTableItem(this, modifier_, FALSE );
	item->setCurrentItem(0);
  setItem(numRows() -1, 0,  item) ;
	item = new QComboTableItem(this, keys_, FALSE );
	item->setCurrentItem(0);
  setItem(numRows() -1, 1,  item) ;
  setText(numRows() -1, 2, "");
	update();
}

void HotkeyTable::removeSelection()
{
	for (Index p = 0; p < numRows(); p++)
	{
		if (isRowSelected(p))
		{
			removeRow(p);
			return;
		}
	}
}

bool HotkeyTable::getValue(String& value) const
{
	value = "";
	for (Position p = 0; p < (Position)numRows(); p++)
	{
		Index c1 = ((QComboTableItem*)item(p, 0))->currentItem();
		Index c2 = ((QComboTableItem*)item(p, 1))->currentItem();
		value += String(c1) + '°' + String(c2) + '°' + item(p, 2)->text().ascii();
		value += '@';
	}
	return true;
}

bool HotkeyTable::setValue(const String& value)
{
	setNumRows(0);

	vector<String> fields;
	vector<String> fields2;
	Size nr = value.split(fields, "@");

	for (Position p = 0; p < nr; p ++)
	{
		Size nr2 = fields[p].split(fields2, "°");
		if (nr2 != 3)
		{
			BALLVIEW_DEBUG;
			continue;
		}

		Position p0;
		Position p1;
		try
		{
			p0 = fields2[0].toUnsignedShort();
			p1 = fields2[1].toUnsignedShort();
		}
		catch(...)
		{
			BALLVIEW_DEBUG;
			continue;
		}

		appendHotkey((Modifier)p0, p1 + 1, fields2[2].c_str());
	}
	
	return true;
}


void PythonSettings::rowSelected()
{
 	remove_button->setEnabled(table->currentRow() >= 0);
}

const List<Hotkey> PythonSettings::getContent() const
	throw()
{
 	return table->getContent();
}

void PythonSettings::setContent(const List<Hotkey>& hotkeys)
{
 	table->setContent(hotkeys);
}

PythonSettings::PythonSettings( QWidget* parent,  const char* name, WFlags fl )
 : PythonSettingsData(parent, name, fl),
	 PreferencesEntry()
{
	setINIFileSectionName("PYTHON");

	table = new HotkeyTable(widget_stack->widget(1));
	connect(new_button, SIGNAL(pressed()), table, SLOT(addEmptyRow()));
	connect(remove_button, SIGNAL(pressed()), table, SLOT(removeSelection()));

	registerWidgetForHelpSystem_(widget_stack->widget(0), "pythonInterpreter.html#startup_script");
	registerWidgetForHelpSystem_(widget_stack->widget(1), "pythonInterpreter.html#create_hotkeys");

	registerObject_(table);
	registerObject_(script_edit);

	setWidgetStackName("Python");
	setWidgetStack(widget_stack);
}


void PythonSettings::fileSelected()
{
	QString s = QFileDialog::getSaveFileName(
								"",
								"Python scripts (*.py)",
								this,	
								"Choose a Startup Python Script",
								"" );

	if (s == QString::null) return;
	script_edit->setText(s);
	QWidget::update();
}

void PythonSettings::setFilename(const String& filename)
	throw()
{
	script_edit->setText(filename.c_str());
	QWidget::update();
}

void PythonSettings::clearStartupScript()
{
	script_edit->setText("");
}

String PythonSettings::getFilename() const
	throw()
{
	return String(script_edit->text().ascii());
}


// NAMESPACE
} }
