// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: mainControlPreferences.C,v 1.14 2004/09/30 15:51:21 amoll Exp $
//

#include <BALL/VIEW/DIALOGS/mainControlPreferences.h>
#include <BALL/VIEW/KERNEL/common.h>

#include <qcombobox.h>
#include <qcheckbox.h>
#include <qstylefactory.h>

namespace BALL
{
	namespace VIEW
	{

MainControlPreferences::MainControlPreferences(QWidget* parent, const char* name, WFlags fl)
	throw()
	: MainControlPreferencesData(parent, name, fl),
		PreferencesEntry()
{
	setINIFileSectionName("GENERAL");
	style_box_->insertStringList(QStyleFactory::keys());
	registerObject_(style_box_);
	registerObject_(show_labels);
	registerObject_(logging_to_file);

	insertEntry(this, "General");
}

MainControlPreferences::~MainControlPreferences()
	throw()
{
	#ifdef BALL_VIEW_DEBUG
		Log.error() << "Destructing object " << (void *)this 
								<< " of class MainControlPreferences" << endl;
	#endif 
}

QStyle* MainControlPreferences::getStyle()
	throw()
{
	QStyle* new_style = QStyleFactory::create(style_box_->currentText());			
	return new_style;
}

bool MainControlPreferences::showLabelsEnabled() const
	throw()
{
	return show_labels->isChecked();
}

void MainControlPreferences::enableLoggingToFile(bool state)
	throw()
{
	if (state) logging_to_file->setChecked(true);
	else       logging_to_file->setChecked(false);
}

bool MainControlPreferences::loggingToFileEnabled() const
	throw()
{
	return logging_to_file->isChecked();
}

} } // namespaces
