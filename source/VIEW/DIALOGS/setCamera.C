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

	view_x->setText(String((Index)camera.getViewPoint().x).c_str());
	view_y->setText(String((Index)camera.getViewPoint().y).c_str());
	view_z->setText(String((Index)camera.getViewPoint().z).c_str());

	look_x->setText(String((Index)camera.getLookAtPosition().x).c_str());
	look_y->setText(String((Index)camera.getLookAtPosition().y).c_str());
	look_z->setText(String((Index)camera.getLookAtPosition().z).c_str());
}

SetCamera::~SetCamera()
{
 // no need to delete child widgets, Qt does it all for us
}

void SetCamera::okPressed()
{
	MainControl *main_control = MainControl::getMainControl(parentWidget());
	hide();

	float vx, vy, vz, lx, ly, lz;

	try
	{
		vx = String(view_x->text().ascii()).toFloat();
		vy = String(view_y->text().ascii()).toFloat();
		vz = String(view_z->text().ascii()).toFloat();
		lx = String(look_x->text().ascii()).toFloat();
		ly = String(look_y->text().ascii()).toFloat();
		lz = String(look_z->text().ascii()).toFloat();
	}
	catch(...)
	{
		main_control->setStatusbarText("Invalid Values!");
		return;
	}

	Vector3 vp(vx,vy,vz);
	Vector3 lp(lx,ly,lz);

	if (vp == lp) 
	{
		Log.error() << "Invalid values for setCamera: viewpoint = look at" << std::endl;
		return;
	}

	Camera& camera = ((Scene*) parentWidget())->getStage()->getCamera();
	camera.setViewPoint(vp);
	camera.setLookAtPosition(lp); 
}

// NAMESPACE
} }
