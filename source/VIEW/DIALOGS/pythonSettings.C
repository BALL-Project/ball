// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#include <BALL/VIEW/DIALOGS/pythonSettings.h>

#include <qlineedit.h>
#include <qfiledialog.h>

namespace BALL
{
	namespace VIEW
	{

PythonSettings::PythonSettings( QWidget* parent,  const char* name, WFlags fl )
 : PythonSettingsData( parent, name, fl )
{
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

String PythonSettings::getFilename() const
	throw()
{
	return String(script_edit->text().ascii());
}


// NAMESPACE
} }
