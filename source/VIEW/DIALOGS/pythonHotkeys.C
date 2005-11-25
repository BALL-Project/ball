// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#include <BALL/VIEW/DIALOGS/pythonHotkeys.h>
#include <BALL/VIEW/KERNEL/mainControl.h>

#include <qtable.h>
#include <qpushbutton.h>

namespace BALL
{
	namespace VIEW
	{

HotkeyTable::HotkeyTable(QWidget* parent,  const char*)
{
  horizontalHeader()->setLabel(0, "Modifier");
  horizontalHeader()->setLabel(1, "Key");
  horizontalHeader()->setLabel(2, "Command");

  setNumRows(0);
  setNumCols(3);
	setShowGrid(true);
	
	setColumnWidth(2, 240);
	setGeometry(5,5, 546, 330);

	setSelectionMode(QTable::SingleRow);

	setName("PythonHotkeys");

	modifier_ << "None" << "Shift" << "Alt";

	for (Position p = 1; p < 13; p++)
	{
		keys_ << (String("F") + String(p)).c_str();
	}
	
	// F2 -> runScriptAgain()
	addEmptyRow();
	((QComboTableItem*)item(0,0))->setCurrentItem(0);
	((QComboTableItem*)item(0,1))->setCurrentItem(1);
	item(0,2)->setText("runScriptAgain()");

	// F3 -> clearRepresentations()
	addEmptyRow();
	((QComboTableItem*)item(1,0))->setCurrentItem(0);
	((QComboTableItem*)item(1,1))->setCurrentItem(2);
	item(1,2)->setText("clearRepresentations()");

	// F4 -> removeWater()
	addEmptyRow();
	((QComboTableItem*)item(2,0))->setCurrentItem(0);
	((QComboTableItem*)item(2,1))->setCurrentItem(3);
	item(2,2)->setText("removeWater()");
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
				hotkey.button_state =Qt::ShiftButton;
				break;

			case 2:
				hotkey.button_state = Qt::AltButton;
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

			case (Position)Qt::AltButton:
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
	throw()
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
	for (Position p = 0; p < numRows(); p++)
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

		addEmptyRow();

		((QComboTableItem*)item(p,0))->setCurrentItem(p0);
		((QComboTableItem*)item(p,1))->setCurrentItem(p1);
		item(p,2)->setText(fields2[2].c_str());
	}
	
	return true;
}



PythonHotkeys::PythonHotkeys( QWidget* parent,  const char* name, WFlags fl )
  : PythonHotkeysData( parent, name, fl ),
		PreferencesEntry()
{
	setWidgetStackName("Python Hotkeys");
	registerWidgetForHelpSystem_(this, "pythonInterpreter.html#create_hotkeys");

	table = new HotkeyTable(this);
	connect(new_button, SIGNAL(pressed()), table, SLOT(addEmtpyRow()));
	connect(remove_button, SIGNAL(pressed()), table, SLOT(removeSelection()));
	connect(table, SIGNAL(selectionChanged()), this, SLOT(rowSelected()));
}


void PythonHotkeys::rowSelected()
{
	remove_button->setEnabled(table->currentSelection() == -1);
}

const List<Hotkey>& PythonHotkeys::getContent() const
	throw()
{
	return table->getContent();
}

void PythonHotkeys::setContent(const List<Hotkey>& hotkeys)
{
	table->setContent(hotkeys);
}



} } // NAMESPACE
