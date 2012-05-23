// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#include <BALL/VIEW/DIALOGS/parsedFunctionDialog.h>
#include <BALL/VIEW/WIDGETS/scene.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/stage.h>

#include <QtWidgets/QPushButton>
#include <QtWidgets/QLineEdit>

namespace BALL
{
	namespace VIEW
	{

ParsedFunctionDialog::ParsedFunctionDialog( QWidget* parent,  const char* name)
    : QDialog(parent),
			Ui_ParsedFunctionDialogData()
{
	setupUi(this);
	setObjectName(name);
	
  // signals and slots connections
  connect( buttonOk, SIGNAL( clicked() ), this, SLOT( accept() ) );
  connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );

	show();
}

ParsedFunctionDialog::~ParsedFunctionDialog()
{
 // no need to delete child widgets, Qt does it all for us
}

// NAMESPACE
} }
