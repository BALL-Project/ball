/**********************************************************************

	--- Qt Architect generated file ---

	File: DlgPreferencesDisplay.C
	Last generated: Fri Jan 14 13:15:10 2000

 *********************************************************************/

#include "DlgPreferencesDisplay.h"
#include <qcolordialog.h>
#define Inherited DlgPreferencesDisplayData

DlgPreferencesDisplay::DlgPreferencesDisplay
(
	QWidget* parent,
	const char* name
)
	:
	Inherited( parent, name )
{
}

DlgPreferencesDisplay::~DlgPreferencesDisplay()
{
}

void DlgPreferencesDisplay::editColor()
{
  color_sample_->setBackgroundColor(QColorDialog::getColor(color_sample_->backgroundColor()));
}
