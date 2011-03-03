// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: mainControlPreferences.C,v 1.16.16.1 2007/03/25 22:02:03 oliver Exp $
//

#include <BALL/VIEW/DIALOGS/mainControlPreferences.h>
#include <BALL/VIEW/KERNEL/common.h>

#include <QtCore/QDir>

#include <QtGui/QCheckBox>
#include <QtGui/QStyleFactory>
#include <QtGui/QFontDialog>

#include <BALL/SYSTEM/path.h>

namespace BALL
{
	namespace VIEW
	{

MainControlPreferences::MainControlPreferences(QWidget* parent, const char* name, Qt::WFlags fl)
	: QWidget(parent, fl),
		Ui_MainControlPreferencesData(),
		PreferencesEntry(),
		last_index_(0)
{
	setupUi(this);
	setObjectName(name);
	setINIFileSectionName("GENERAL");

	Path p;
	QStringList dpaths = QString(p.getDataPath().c_str()).split("\n");

	foreach(QString str, dpaths) {
		QDir dir(str + "BALLView/translations");
		QStringList tList = dir.entryList(QStringList("BALLView.*.qm"));
		foreach(QString entry, tList) {
			entry.replace("BALLView.", "");
			entry.replace(".qm", "");
			languageComboBox_->addItem(entry);
		}
	}

	style_box_->addItems(QStyleFactory::keys());
	QString prefered_style = "Plastique";
#ifdef BALL_OS_WINDOWS
	prefered_style = "WindowsXP";
#endif
#ifdef BALL_OS_DARWIN
	prefered_style = "Macintosh (aqua)";
#endif

	Index pos = style_box_->findText(prefered_style);
	if (pos != -1) style_box_->setCurrentIndex(pos);

	setWidgetStackName("General");
	registerWidgets_();
	unregisterObject_(style_box_);
	unregisterObject_(languageComboBox_);

	connect( font_button, SIGNAL( clicked() ), this, SLOT( selectFont() ) );
}

MainControlPreferences::~MainControlPreferences()
{
	#ifdef BALL_VIEW_DEBUG
		Log.error() << "Destructing object " << (void *)this 
								<< " of class MainControlPreferences" << endl;
	#endif 
}

QStyle* MainControlPreferences::setStyle()
{
	if(!QApplication::style() || (QApplication::style()->objectName().compare(style_box_->currentText(), Qt::CaseInsensitive) != 0))
	{
		QStyle* new_style = QStyleFactory::create(style_box_->currentText());
		QApplication::setStyle(new_style);
	}

	return QApplication::style();
}

QFont MainControlPreferences::getFont()
{
	return font_label->font();
}

void MainControlPreferences::enableLoggingToFile(bool state)
{
	if (state) logging_to_file->setChecked(true);
	else       logging_to_file->setChecked(false);
}

bool MainControlPreferences::loggingToFileEnabled() const
{
	return logging_to_file->isChecked();
}

void MainControlPreferences::selectFont()
{
	bool ok = true;
	QFont font = QFontDialog::getFont(&ok, font_, 0);

	if (!ok) return;

	font_label->setFont(font);
	font_ = font;
}

void MainControlPreferences::setFont(QFont font)
{
	font_ = font;
	font_label->setFont(font);
}

void MainControlPreferences::writePreferenceEntries(INIFile& inifile)
{
	PreferencesEntry::writePreferenceEntries(inifile);
	inifile.insertValue(inifile_section_name_, "style", ascii(style_box_->currentText()));
	inifile.insertValue(inifile_section_name_, "language", ascii(languageComboBox_->currentText()));
}

void MainControlPreferences::readPreferenceEntries(const INIFile& inifile)
{
	PreferencesEntry::readPreferenceEntries(inifile);
	if (inifile.hasEntry(inifile_section_name_, "style"))
	{
		String value = inifile.getValue(inifile_section_name_, "style");
		int e = style_box_->findText(value.c_str());
		if (e == -1) return;

		style_box_->setCurrentIndex(e);
	}

	if (inifile.hasEntry(inifile_section_name_, "language"))
	{
		String value = inifile.getValue(inifile_section_name_, "language");
		int e = languageComboBox_->findText(value.c_str());
		if (e == -1) return;

		last_index_ = e;
		languageComboBox_->setCurrentIndex(e);
	}
}

} } // namespaces
