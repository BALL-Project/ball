// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#include <BALL/VIEW/DIALOGS/modelSettingsDialog.h>

#include <qlineedit.h>

namespace BALL
{
	namespace VIEW
	{

ModelSettingsDialog::ModelSettingsDialog( QWidget* parent,  const char* name, WFlags fl )
    : ModelSettingsDialogData( parent, name, fl )
{
	show();
}

void ModelSettingsDialog::apply()
	throw()
{
}

} } // NAMESPACE
