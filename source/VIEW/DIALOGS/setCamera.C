// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#include <BALL/VIEW/DIALOGS/setCamera.h>
#include <BALL/VIEW/WIDGETS/scene.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/stage.h>

#include <qpushbutton.h>
#include <qlineedit.h> 

namespace BALL
{
	namespace VIEW
	{

SetCamera::SetCamera( QWidget* parent,  const char* name, bool modal, WFlags fl )
    : SetCameraData( parent, name, modal, fl )
{
	const Camera& camera = ((Scene*) parent)->getStage()->getCamera();

	String text(String(camera.getViewPoint().x) + "|" +
							String(camera.getViewPoint().y) + "|" +
							String(camera.getViewPoint().z));
	view_edit->setText(text.c_str());

	text = String(camera.getLookAtPosition().x) + "|" +
				 String(camera.getLookAtPosition().y) + "|" +
				 String(camera.getLookAtPosition().z);

	look_edit->setText(text.c_str());

	show();
}

SetCamera::~SetCamera()
{
 // no need to delete child widgets, Qt does it all for us
}

void SetCamera::okPressed()
{
	MainControl *main_control = MainControl::getMainControl(parentWidget());

	hide();
	vector<String> strings1, strings2;
	String(view_edit->text().ascii()).split(strings1, "|");
	String(look_edit->text().ascii()).split(strings2, "|");
	
	if (strings1.size() != 3 ||
			strings2.size() != 3) 
	{
		main_control->setStatusbarText("Invalid Values!");
		return;
	}

	for (Index i = 0; i<3; i++)
	{
		if (!strings1[i].isFloat() ||
				!strings2[i].isFloat())
		{
			main_control->setStatusbarText("Invalid Values!");
			return;
		}
	}

	Camera& camera = ((Scene*) parentWidget())->getStage()->getCamera();
	camera.setViewPoint(Vector3(strings1[0].toFloat(), strings1[1].toFloat(), strings1[2].toFloat()));
	camera.setLookAtPosition(Vector3(strings2[0].toFloat(), strings2[1].toFloat(), strings2[2].toFloat()));
}

// NAMESPACE
} }
