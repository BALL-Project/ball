/**********************************************************************

	--- Qt Architect generated file ---

	File: DlgDisplayProperties.C
	Last generated: Mon Jan 10 18:55:27 2000

 *********************************************************************/

#include <qcolordialog.h>
#include "DlgDisplayProperties.h"

#define Inherited DlgDisplayPropertiesData

DlgDisplayProperties::DlgDisplayProperties
(
	QWidget* parent,
	const char* name
)
	:
	Inherited( parent, name )
{
    setCaption("Display Settings");
}

DlgDisplayProperties::~DlgDisplayProperties()
{
}

void DlgDisplayProperties::editColor()
{
	color_sample->setBackgroundColor(QColorDialog::getColor(color_sample->backgroundColor()));
}
