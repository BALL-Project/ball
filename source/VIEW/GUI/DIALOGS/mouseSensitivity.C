// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#include <BALL/VIEW/GUI/DIALOGS/mouseSensitivity.h>

#include <BALL/VIEW/GUI/WIDGETS/scene.h>

#include <qpushbutton.h>
#include <qslider.h> 

namespace BALL
{
	namespace VIEW
	{

/* 
 *  Constructs a MouseSensitivity which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
MouseSensitivity::MouseSensitivity( QWidget* parent,  const char* name, bool modal, WFlags fl )
    : MouseSensitivityData( parent, name, modal, fl )
{
	slider_->setValue((int)Scene::getMouseSensitivity() - 1);
	show();
}

MouseSensitivity::~MouseSensitivity()
{
 // no need to delete child widgets, Qt does it all for us
}

void MouseSensitivity::okPressed()
{
	Scene::setMouseSensitivity(slider_->value() + 1);
	hide();
}

void MouseSensitivity::cancelPressed()
{
	hide();
	close();
}

// NAMESPACE
	}
}
