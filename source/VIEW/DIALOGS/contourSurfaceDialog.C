// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#include <BALL/VIEW/DIALOGS/contourSurfaceDialog.h>

#include <qframe.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qvariant.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qfiledialog.h>

namespace BALL
{
 namespace VIEW
 {

ContourSurfaceDialog::ContourSurfaceDialog( QWidget* parent,  const char* name )
    : ContourSurfaceDialogData( parent, name, TRUE, 0 )
{
}

ContourSurfaceDialog::~ContourSurfaceDialog()
{
  // no need to delete child widgets, Qt does it all for us
}

double ContourSurfaceDialog::getThreshold() const	
{
	return (double)String(threshold->text().ascii()).toFloat();
}

String ContourSurfaceDialog::getFileName() const
{
	return filename->text().ascii();
}

void ContourSurfaceDialog::fileNameChanged()
{
	buttonOk->setEnabled(getFileName() != "");
}

void ContourSurfaceDialog::browseFiles()
{
	// look up the full path of the parameter file
	Path p;
	String file = p.find(getFileName());
	if (file == "")
	{
		file = getFileName();
	}
	QString tmp = file.c_str();
	QString result = QFileDialog::getOpenFileName(tmp, "*", 0, "Select a RegularData file");
	if (!result.isEmpty())
	{
		// store the new filename in the lineedit field
		filename->setText(result);
	}
}

}} //namespaces
