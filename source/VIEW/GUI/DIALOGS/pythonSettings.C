// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#include <BALL/VIEW/GUI/DIALOGS/pythonSettings.h>

#include <qlineedit.h>
#include <qfiledialog.h>

namespace BALL
{
	namespace VIEW
	{

PythonSettings::PythonSettings( QWidget* parent,  const char* name, WFlags fl )
 : PythonSettingsData( parent, name, fl )
{
	show();
}


void PythonSettings::fileSelected()
{
	QFileDialog *fd = new QFileDialog(this, "Choose Python Script", true);
	fd->setMode(QFileDialog::ExistingFile);
	fd->addFilter("Python scripts (*.py)");
	fd->setSelectedFilter(1);

	fd->setCaption("Choose Python Script");
	fd->setViewMode(QFileDialog::Detail);
	fd->setGeometry(300, 150, 400, 400);

	int result_dialog = fd->exec();
	if (!result_dialog == QDialog::Accepted) return;

	script_edit->setText(fd->selectedFile());
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
