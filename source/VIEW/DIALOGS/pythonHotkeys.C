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
    : PythonHotkeysData( parent, name, fl )
{
  table->horizontalHeader()->setLabel(0, "Modifier");
  table->horizontalHeader()->setLabel(1, "Key");
  table->horizontalHeader()->setLabel(2, "Command");

  table->setNumRows(0);
	table->setColumnWidth(2, 240);

	modifier_ << "None" << "Shift" << "Ctrl" << "Alt";
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
			continue;
		}

		Hotkey hotkey;

		table->item(pos, 0);
		Index p = (dynamic_cast<QComboTableItem*>( table->item(pos, 0)))->currentItem();
		if (p == 0)
		{
			hotkey.button_state = Qt::NoButton;
		}
		else
		{
			hotkey.button_state = (Qt::ButtonState) (Qt::ShiftButton + p - 1);
		}

		p = ((QComboTableItem*) table->item(p, 1))->currentItem();
		hotkey.key = (Qt::Key) (Qt::Key_F1 + p);

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
		if ((*it).button_state == Qt::NoButton ||
				(*it).button_state == 0)
		{
			item->setCurrentItem(0);
		}
		else
		{
			if ((*it).button_state < Qt::ShiftButton) 
			{
				Log.error() << "Invalid button state for Hotkey" << std::endl;
			}
			else
			{
				item->setCurrentItem((*it).button_state - Qt::ShiftButton + 1);
			}
		}
    table->setItem(p, 0,  item) ;

		QComboTableItem * item2 = new QComboTableItem(table, keys_, FALSE );
		item2->setCurrentItem((*it).key - Qt::Key_F1);
    table->setItem(p, 1,  item2) ;

    table->setText(p, 2,  (*it).action.c_str());

		p++;
	}
}

				
void PythonHotkeys::setDefaults(bool all)
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
