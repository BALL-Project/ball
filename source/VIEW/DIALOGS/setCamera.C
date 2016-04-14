// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
//
#include <BALL/VIEW/DIALOGS/setCamera.h>
#include <BALL/VIEW/WIDGETS/scene.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/stage.h>

#include <QtWidgets/QPushButton>
#include <QtWidgets/QLineEdit>

namespace BALL
{
	namespace VIEW
	{

SetCamera::SetCamera( QWidget* parent,  const char* name, bool modal, Qt::WindowFlags fl )
	: QDialog(parent, fl),
		Ui_SetCameraData()
{
	setupUi(this);
	
  // signals and slots connections
  connect( buttonBox, SIGNAL( accepted() ), this, SLOT( okPressed() ) );
  connect( buttonBox, SIGNAL( rejected() ), this, SLOT( close() ) );

	setModal(modal);
	setObjectName(name);
	const Camera& camera = ((Scene*) parent)->getStage()->getCamera();

	view_x->setText(QString::number(camera.getViewPoint().x, 'f', 3));
	view_y->setText(QString::number(camera.getViewPoint().y, 'f', 3));
	view_z->setText(QString::number(camera.getViewPoint().z, 'f', 3));

	look_x->setText(QString::number(camera.getLookAtPosition().x, 'f', 3));
	look_y->setText(QString::number(camera.getLookAtPosition().y, 'f', 3));
	look_z->setText(QString::number(camera.getLookAtPosition().z, 'f', 3));
	
	look_up_x->setText(QString::number(camera.getLookUpVector().x, 'f', 3));
	look_up_y->setText(QString::number(camera.getLookUpVector().y, 'f', 3));
	look_up_z->setText(QString::number(camera.getLookUpVector().z, 'f', 3));
}

SetCamera::~SetCamera()
{
 // no need to delete child widgets, Qt does it all for us
}

void SetCamera::okPressed()
{
	MainControl *main_control = MainControl::getMainControl(parentWidget());
	hide();

	float vx, vy, vz, lx, ly, lz, lux, luy, luz;

	try
	{
		vx = (view_x->text()).toFloat();
		vy = (view_y->text()).toFloat();
		vz = (view_z->text()).toFloat();
		
		lx = (look_x->text()).toFloat();
		ly = (look_y->text()).toFloat();
		lz = (look_z->text()).toFloat();
		
		lux = (look_up_x->text()).toFloat();
		luy = (look_up_y->text()).toFloat();
		luz = (look_up_z->text()).toFloat();
	}
	catch(...)
	{
		main_control->setStatusbarText((String)tr("Invalid Values!"));
		return;
	}

	Vector3 vp(vx,vy,vz);
	Vector3 lp(lx,ly,lz);
	Vector3 lu(lux,luy,luz);

	if (vp == lp) 
	{
		Log.error() << (String)tr("Invalid values for setCamera: viewpoint = look at") << std::endl;
		return;
	}

	Camera& camera = ((Scene*) parentWidget())->getStage()->getCamera();
	camera.setViewPoint(vp);
	camera.setLookAtPosition(lp); 
	camera.setLookUpVector(lu); 
}

// NAMESPACE
} }
