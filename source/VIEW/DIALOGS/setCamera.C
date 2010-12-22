// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
//
#include <BALL/VIEW/DIALOGS/setCamera.h>
#include <BALL/VIEW/WIDGETS/scene.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/stage.h>

#include <QtGui/QPushButton>
#include <QtGui/QLineEdit> 

namespace BALL
{
	namespace VIEW
	{

SetCamera::SetCamera( QWidget* parent,  const char* name, bool modal, Qt::WFlags fl )
	: QDialog(parent, fl),
		Ui_SetCameraData()
{
	setupUi(this);
	
  // signals and slots connections
  connect( ok_button, SIGNAL( pressed() ), this, SLOT( okPressed() ) );
  connect( cancel_button, SIGNAL( pressed() ), this, SLOT( close() ) );

	setModal(modal);
	setObjectName(name);
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
		vx = ascii(view_x->text()).toFloat();
		vy = ascii(view_y->text()).toFloat();
		vz = ascii(view_z->text()).toFloat();
		lx = ascii(look_x->text()).toFloat();
		ly = ascii(look_y->text()).toFloat();
		lz = ascii(look_z->text()).toFloat();
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
