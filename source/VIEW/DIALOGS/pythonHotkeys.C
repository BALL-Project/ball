// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#include <BALL/VIEW/DIALOGS/pythonHotkeys.h>

#include <qtable.h>
#include <qpushbutton.h>

namespace BALL
{
	namespace VIEW
	{

PythonHotkeys::PythonHotkeys( QWidget* parent,  const char* name, WFlags fl )
  : PythonHotkeysData( parent, name, fl ),
		PreferencesEntry()
{
  table->horizontalHeader()->setLabel(0, "Modifier");
  table->horizontalHeader()->setLabel(1, "Key");
  table->horizontalHeader()->setLabel(2, "Command");

  table->setNumRows(0);
	table->setColumnWidth(2, 240);

	//modifier_ << "None" << "Shift" << "Ctrl" << "Alt";
	modifier_ << "None" << "Shift" << "Alt";
	for (Position p = 1; p < 13; p++)
	{
		keys_ << String("F") + String(p).c_str();
	}
	
	setDefaults();
}


List<Hotkey> PythonHotkeys::getContent() const
	throw()
{
	List<Hotkey> result;
	for (Index pos = 0; pos < table->numRows(); pos++)
	{
		if (table->item(pos, 2)->text().isEmpty() ||
				!RTTI::isKindOf<QComboTableItem>(*table->item(pos, 0))) 
		{
			Log.error() << "Problem reading content of PythonHotkeys" << std::endl;
			continue;
		}

		Hotkey hotkey;

		Index ci = ((QComboTableItem*) table->item(pos, 0))->currentItem();
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

		ci = ((QComboTableItem*) table->item(pos, 1))->currentItem();
		hotkey.key = (Qt::Key) (Qt::Key_F1 + ci);

		hotkey.action = table->item(pos, 2)->text().ascii();

		result.push_back(hotkey);
	}

	return result;
}

void PythonHotkeys::setContent(const List<Hotkey>& hotkeys)
	throw()
{
  table->setNumRows(hotkeys.size());
	table->setColumnWidth(2, 230);

	List<Hotkey>::ConstIterator it = hotkeys.begin();
	Position p = 0;
	for (; it != hotkeys.end(); it++)
	{
		QComboTableItem * item = new QComboTableItem(table, modifier_, FALSE );
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
    table->setItem(p, 0,  item) ;

		QComboTableItem * item2 = new QComboTableItem(table, keys_, FALSE );
		item2->setCurrentItem((*it).key - Qt::Key_F1);
    table->setItem(p, 1,  item2) ;

    table->setText(p, 2,  (*it).action.c_str());

		p++;
	}
}

				
void PythonHotkeys::setDefaults(bool /*all*/)
	throw()
{
}

void PythonHotkeys::addEmtpyLine_()
	throw()
{
	table->setNumRows(table->numRows() + 1);
	QComboTableItem * item = new QComboTableItem(table, modifier_, FALSE );
	item->setCurrentItem(0);
  table->setItem(table->numRows() -1, 0,  item) ;
	item = new QComboTableItem(table, keys_, FALSE );
	item->setCurrentItem(0);
  table->setItem(table->numRows() -1, 1,  item) ;
  table->setText(table->numRows() -1, 2, "");
	table->update();
}

void PythonHotkeys::removePressed()
{
	for (Index p = 0; p < table->numRows(); p++)
	{
		if (table->isRowSelected(p))
		{
			table->removeRow(p);
			return;
		}
	}
}

void PythonHotkeys::newPressed()
{
	addEmtpyLine_();
}

void PythonHotkeys::rowSelected()
{
	remove_button->setEnabled(table->currentSelection() == -1);
}


} } // NAMESPACE
