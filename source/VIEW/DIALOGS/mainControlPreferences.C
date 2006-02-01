// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: mainControlPreferences.C,v 1.16.2.3 2006/02/01 13:23:46 amoll Exp $
//

#include <BALL/VIEW/DIALOGS/mainControlPreferences.h>
#include <BALL/VIEW/KERNEL/common.h>

#include <QtGui/qcheckbox.h>
#include <QtGui/qstylefactory.h>

namespace BALL
{
	namespace VIEW
	{

MainControlPreferences::MainControlPreferences(QWidget* parent, const char* name, Qt::WFlags fl)
	throw()
	: QWidget(parent, fl),
		Ui_MainControlPreferencesData(),
		PreferencesEntry()
{
	setupUi(this);
	setObjectName(name);
	setINIFileSectionName("GENERAL");
	style_box_->addItems(QStyleFactory::keys());
	registerObject_(style_box_);
	registerObject_(logging_to_file);

	setWidgetStackName("General");
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
