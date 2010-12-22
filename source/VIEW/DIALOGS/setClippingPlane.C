// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/DIALOGS/setClippingPlane.h>
#include <BALL/VIEW/KERNEL/mainControl.h>

#include <QtGui/QPushButton>
#include <QtGui/QLineEdit> 

namespace BALL
{
	namespace VIEW
	{

SetClippingPlane::SetClippingPlane( QWidget* parent,  const char* name, ClippingPlane* plane)
	: QDialog(parent),
		Ui_SetClippingData()
{
	setupUi(this);
	
  // signals and slots connections
  connect( ok_button, SIGNAL( pressed() ), this, SLOT( okPressed() ) );
  connect( cancel_button, SIGNAL( pressed() ), this, SLOT( close() ) );

	setObjectName(name);
	plane_ = plane;

	if (plane_ == 0) return;

	Vector3 p = plane_->getPoint();
	view_x->setText(String(p.x).c_str());
	view_y->setText(String(p.y).c_str());
	view_z->setText(String(p.z).c_str());

	p = plane_->getNormal();
	look_x->setText(String(p.x).c_str());
	look_y->setText(String(p.y).c_str());
	look_z->setText(String(p.z).c_str());
}

SetClippingPlane::~SetClippingPlane()
{
 // no need to delete child widgets, Qt does it all for us
}

void SetClippingPlane::okPressed()
{
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
		getMainControl()->setStatusbarText((String)tr("Invalid Values!"));
		return;
	}

	Vector3 vp(vx,vy,vz);
	Vector3 lp(lx,ly,lz);

	if (!Maths::isZero(lp.getLength()))
	{
		lp.normalize();
	}
	else
	{
		lp = Vector3(1,0,0);
	}

	if (plane_ != 0)
	{
		plane_->setPoint(vp);
		plane_->setNormal(lp);
	}
	else
	{
		getMainControl()->setStatusbarText((String)tr("No plane available"));
	}

	close();
}

// NAMESPACE
} }
