// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#include <BALL/VIEW/DIALOGS/pythonSettings.h>
#include <BALL/VIEW/KERNEL/mainControl.h>

#include <qlineedit.h>
#include <qfiledialog.h>
#include <qpushbutton.h>
#include <qstackedwidget.h>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QComboBox>

namespace BALL
{
	namespace VIEW
	{

ComboBoxDelegate::ComboBoxDelegate(QObject* parent)
	: QItemDelegate(parent)
{
	sl_modifier_ << "None" << "Shift" << "Ctrl";
	
	for (Position p = 2; p < 13; p++)
	{
		sl_keys_ << (String("F") + String(p)).c_str();
	}
}

QWidget *ComboBoxDelegate::createEditor(QWidget *parent,
		const QStyleOptionViewItem& option,
		const QModelIndex& index) const
{
	Position c = index.column();

	if (c == 2) return QItemDelegate::createEditor(parent, option, index);

	QComboBox* editor = new QComboBox(parent);

	if (c == 0) editor->addItems(sl_modifier_);
	else 			 editor->addItems(sl_keys_);

	editor->setEditable(false);
	editor->setFrame(false);
	editor->setDuplicatesEnabled(false);

	editor->installEventFilter(const_cast<ComboBoxDelegate*>(this));
	editor->showPopup();
	return editor;
}

void ComboBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	if (index.column() == 2)
	{
		QItemDelegate::setEditorData(editor, index);
		return;
	}

	QString data = index.model()->data(index, Qt::DisplayRole).toString();
	QComboBox* cb = static_cast<QComboBox*>(editor);
	cb->setCurrentIndex(cb->findText(data));
}

void ComboBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
																	 const QModelIndex &index) const
{
	if (index.column() == 2)
	{
		QItemDelegate::setModelData(editor, model, index);
		return;
	}

	QComboBox* cb = static_cast<QComboBox*>(editor);
	QString data = cb->currentText();
	model->setData(index, data);
}

void ComboBoxDelegate::updateEditorGeometry(QWidget *editor,
												const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
	editor->setGeometry(option.rect);
}

// =====================================================

HotkeyTable::HotkeyTable(QWidget* parent,  const char*)
	throw()
	: QTableWidget(parent),
		delegate_(this)
{
	setItemDelegate(&delegate_);

  setColumnCount(3);
	setHorizontalHeaderItem(0, new QTableWidgetItem());
	setHorizontalHeaderItem(1, new QTableWidgetItem());
	setHorizontalHeaderItem(2, new QTableWidgetItem());
  horizontalHeaderItem(0)->setText("Modifier");
  horizontalHeaderItem(1)->setText("Key");
  horizontalHeaderItem(2)->setText("Command");

  setRowCount(0);
	setShowGrid(true);
	
	setColumnWidth(0, 70);
	setColumnWidth(1, 60);
	setColumnWidth(2, 350);
	setGeometry(2,2, 530, 342);

	setSelectionBehavior(QAbstractItemView::SelectRows);
	setSelectionMode(QAbstractItemView::SingleSelection);

	setObjectName("PythonHotkeys");

	modifier_ << "" << "Shift";// << "Alt";

	for (Position p = 2; p < 13; p++)
	{
		keys_ << (String("F") + String(p)).c_str();
	}
	
	// F2 -> runScriptAgain()
	appendHotkey("None", "F2", "runScriptAgain()");

	// F3 -> hideAllRepresentations()
	appendHotkey("None", "F3", "hideAllRepresentations()");

	// F4 -> removeWater()
	appendHotkey("None", "F4", "removeWater()");

	// F5 -> printAtomTypesForHighlighted()
	appendHotkey("None", "F5", "printAtomTypesForHighlighted()");

	// F5 -> printAtomTypesForLigands()
	appendHotkey("Shift", "F5", "printAtomTypesForLigands()");

	// F6 -> showCartoonAndLigand()
	appendHotkey("None", "F6", "highlightLigand()");

	// S-F6 -> showCartoonAndLigand()
	appendHotkey("Shift", "F6", "showCartoonAndLigand()");

	// F7 -> addOptimizedHydrogens()
	appendHotkey("None", "F7", "addOptimizedHydrogens()");
	
	// S-F7 -> relaxStructure()
	appendHotkey("Shift", "F7", "relaxStructure()");
	
	// F8 -> quickSave()
	appendHotkey("None", "F8", "quickSave()");
	
	// S-F8 -> quickLoad()
	appendHotkey("Shift", "F8", "quickLoad()");
	
	// F12 -> clearRepresentations()
	appendHotkey("None", "F12", "clearRepresentations()");
	
	// S-F12 -> clearAll()
	appendHotkey("Shift", "F12", "clearAll()");
}

void HotkeyTable::appendHotkey(const String& mod, const String& F_key, const String& command)
{
	addEmptyRow();
	Index r = rowCount() - 1;

	item(r, 0)->setText(mod.c_str());
	item(r, 1)->setText(F_key.c_str());
	item(r, 2)->setText(command.c_str());
	scrollToBottom();
}

List<Hotkey> HotkeyTable::getContent() const
	throw()
{
	List<Hotkey> result;
	for (Index pos = 0; pos < rowCount(); pos++)
	{
		if (item(pos, 2)->text().isEmpty())
		{
			Log.error() << "Problem reading content of PythonHotkeys" << std::endl;
			continue;
		}

		Hotkey hotkey;

		String s = ascii(item(pos, 0)->text());
		if      (s == "None")  hotkey.button_state = Qt::NoModifier;
		else if (s == "Shift") hotkey.button_state = Qt::ShiftModifier;
		else if (s == "Ctrl")  hotkey.button_state = Qt::ControlModifier;
		else Log.error() << "Problem reading content of PythonHotkeys" << std::endl;

		s = ascii(item(pos, 1)->text());
		Index p = s[1] - 49;

    hotkey.key = (Qt::Key) (Qt::Key_F1 + p);

    hotkey.action = ascii(item(pos, 2)->text());

    result.push_back(hotkey);
  }

  return result;
}


void HotkeyTable::setContent(const List<Hotkey>& hotkeys)
	throw()
{
  setRowCount(hotkeys.size());
	setColumnWidth(2, 230);

	List<Hotkey>::ConstIterator it = hotkeys.begin();
	Position p = 0;
	for (; it != hotkeys.end(); it++)
	{
		String s;
		QStringList sl;
		switch ((Position)(*it).button_state)
		{
			case (Position)Qt::NoButton: 				s = "None"; break;
			case (Position)Qt::ShiftModifier: 	s = "Shift"; break;
			case (Position)Qt::ControlModifier: s = "Ctrl"; break;
			default:
				Log.error() << "Invalid button state for Hotkey" << std::endl;
		}

		insertRow(p);

		itemAt(p, 0)->setText(s.c_str());

		Position p = (*it).key - Qt::Key_F1 + 1;
		s = String(p);
		itemAt(p, 1)->setText(s.c_str());

		itemAt(p, 2)->setText((*it).action.c_str());

		p++;
	}
}

void HotkeyTable::addEmptyRow()
{
	Position p = rowCount();
	insertRow(p);
	scrollToBottom();
	setItem(p, 0, new QTableWidgetItem());
	setItem(p, 1, new QTableWidgetItem());
	setItem(p, 2, new QTableWidgetItem());
	item(p, 0)->setText("None");
	item(p, 1)->setText("F2");
	item(p, 2)->setText("");
	emit(cellActivated(p, 2));
}

void HotkeyTable::removeSelection()
{
	QList<QTableWidgetSelectionRange> l = QTableWidget::selectedRanges();
	if (l.size() == 0) return;

	removeRow((*l.begin()).bottomRow());
}

bool HotkeyTable::getValue(String& value) const
{
	value = "";
	for (Position p = 0; p < (Position)rowCount(); p++)
	{
		value += ascii(item(p, 0)->text()) + '°';
		value += ascii(item(p, 1)->text()) + '°';
		value += ascii(item(p, 2)->text()) + '°';
		value += '@';
	}
	return true;
}

bool HotkeyTable::setValue(const String& value)
{
	setRowCount(0);

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

		appendHotkey(fields2[0], fields2[1], fields2[2]);
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


// =============================================================
PythonSettings::PythonSettings(QWidget* parent, const char* name)
 : QDialog(parent),
	 Ui_PythonSettingsData(),
	 PreferencesEntry()
{
	setINIFileSectionName("PYTHON");
	setupUi(this);
	setObjectName(name);
	
	// signals and slots connections
	connect( choose_button, SIGNAL( clicked() ), this, SLOT( fileSelected() ) );
	connect( clear_button, SIGNAL( clicked() ), this, SLOT( clearStartupScript() ) );

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
								0,
								"Choose a Startup Python Script",
								getMainControl()->getWorkingDir().c_str(),
								"Python scripts (*.py)");

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
	return ascii(script_edit->text());
}


// NAMESPACE
} }
