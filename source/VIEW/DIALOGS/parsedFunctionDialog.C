// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#include <BALL/VIEW/DIALOGS/parsedFunctionDialog.h>
#include <BALL/VIEW/WIDGETS/scene.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/stage.h>

#include <qpushbutton.h>
#include <qlineedit.h> 

namespace BALL
{
	namespace VIEW
	{

ParsedFunctionDialog::ParsedFunctionDialog( QWidget* parent,  const char* name, bool modal, WFlags fl )
    : ParsedFunctionDialogData( parent, name, modal, fl )
{
	show();
}

ParsedFunctionDialog::~ParsedFunctionDialog()
{
 // no need to delete child widgets, Qt does it all for us
}

void ParsedFunctionDialog::accept()
{
ParsedFunctionDialogData::accept();
}

// NAMESPACE
} }
