// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#include <BALL/VIEW/DIALOGS/coloringSettingsDialog.h>

#include <qlineedit.h>

namespace BALL
{
	namespace VIEW
	{

ColoringSettingsDialog::ColoringSettingsDialog( QWidget* parent,  const char* name, WFlags fl )
    : ColoringSettingsDialogData( parent, name, fl )
{
	show();
}

void ColoringSettingsDialog::apply()
	throw()
{
}

} } // NAMESPACE
