// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/DIALOGS/pythonSettings.h>
#include <BALL/VIEW/KERNEL/mainControl.h>

#include <QtWidgets/QLineEdit>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTableWidgetItem>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFontDialog>

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
		sl_keys_ << (String(tr("F")) + String(p)).c_str();
	}
}

QWidget *ComboBoxDelegate::createEditor(QWidget *parent,
		const QStyleOptionViewItem& option,
		const QModelIndex& index) const
{
	Position c = index.column();

	if (c >= 2) return QItemDelegate::createEditor(parent, option, index);

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
	if (index.column() >= 2)
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
	if (index.column() >= 2)
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

void PythonSettings::rowSelected()
{
 	remove_button->setEnabled(table->currentRow() >= 0);
}

const list<Hotkey> PythonSettings::getContent() const
{
 	return table->getContent();
}

void PythonSettings::setContent(const list<Hotkey>& hotkeys)
{
 	table->setContent(hotkeys);
}


// =============================================================
PythonSettings::PythonSettings(QWidget* parent, const char* name)
 : QWidget(parent),
	 Ui_PythonSettingsData(),
	 PreferencesEntry()
{
	setINIFileSectionName("PYTHON");
	setupUi(this);
	setObjectName(name);
	
	// signals and slots connections
	connect( choose_button, SIGNAL( clicked() ), this, SLOT( fileSelected() ) );
	connect( clear_button, SIGNAL( clicked() ), this, SLOT( clearStartupScript() ) );
	connect( font_button, SIGNAL( clicked() ), this, SLOT( selectFont() ) );

	connect(new_button, SIGNAL(pressed()), table, SLOT(addEmptyRow()));
	connect(remove_button, SIGNAL(pressed()), table, SLOT(removeSelection()));

	registerWidgetForHelpSystem_(widget_stack->widget(0), "pythonInterpreter.html#startup_script");
	registerWidgetForHelpSystem_(widget_stack->widget(1), "pythonInterpreter.html#create_hotkeys");

	setWidgetStackName("Python");
	setWidgetStack(widget_stack);
	registerWidgets_();
}


void PythonSettings::fileSelected()
{
	QString s = QFileDialog::getOpenFileName(
								0,
								tr("Choose a Startup Python Script"),
								getMainControl()->getWorkingDir().c_str(),
								tr("Python scripts") + " (*.py)");

	if (s == QString::null) return;
	script_edit->setText(s);
	QWidget::update();
}

void PythonSettings::setFilename(const String& filename)
{
	script_edit->setText(filename.c_str());
	QWidget::update();
}

void PythonSettings::clearStartupScript()
{
	script_edit->setText("");
}

String PythonSettings::getFilename() const
{
	return ascii(script_edit->text());
}

void PythonSettings::selectFont()
{
	bool ok = true;
	QFont font = QFontDialog::getFont(&ok, font_, 0);

	if (!ok) return;

	font_label->setFont(font);
	font_ = font;
}

void PythonSettings::writePreferenceEntries(INIFile& inifile)
{
	PreferencesEntry::writePreferenceEntries(inifile);
	
	// the font size
	inifile.insertValue("PYTHON", "font", ascii(font_.toString()));
}

void PythonSettings::readPreferenceEntries(const INIFile& inifile)
{
	PreferencesEntry::readPreferenceEntries(inifile);

	if (inifile.hasEntry("PYTHON", "font"))
	{
		font_.fromString(inifile.getValue("PYTHON", "font").c_str());
	}
	else
	{
		font_ = QFont("Helvetica", 12);
	}

	font_label->setFont(font_);
}

// NAMESPACE
} }
