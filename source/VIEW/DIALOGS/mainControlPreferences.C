// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: mainControlPreferences.C,v 1.16.16.1 2007/03/25 22:02:03 oliver Exp $
//

#include <BALL/VIEW/DIALOGS/mainControlPreferences.h>
#include <BALL/VIEW/KERNEL/common.h>

#include <QtGui/qcheckbox.h>
#include <QtGui/qstylefactory.h>
#include <QtGui/qfontdialog.h>

namespace BALL
{
	namespace VIEW
	{

MainControlPreferences::MainControlPreferences(QWidget* parent, const char* name, Qt::WFlags fl)
	: QWidget(parent, fl),
		Ui_MainControlPreferencesData(),
		PreferencesEntry()
{
	setupUi(this);
	setObjectName(name);
	setINIFileSectionName("GENERAL");
	style_box_->addItems(QStyleFactory::keys());
	QString prefered_style = "Plastique";
#ifdef BALL_PLATFORM_WINDOWS
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

	connect( font_button, SIGNAL( clicked() ), this, SLOT( selectFont() ) );
}

MainControlPreferences::~MainControlPreferences()
{
	#ifdef BALL_VIEW_DEBUG
		Log.error() << "Destructing object " << (void *)this 
								<< " of class MainControlPreferences" << endl;
	#endif 
}

QStyle* MainControlPreferences::getStyle()
{
	QStyle* new_style = QStyleFactory::create(style_box_->currentText());			
	return new_style;
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
}

} } // namespaces
