// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#include <BALL/VIEW/DIALOGS/animationDialog.h>
#include <BALL/VIEW/WIDGETS/scene.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/stage.h>

#include <qpushbutton.h>
#include <qcheckbox.h>
#include <qlistbox.h>
#include <qspinbox.h>

namespace BALL
{
	namespace VIEW
	{

AnimationDialog::AnimationDialog( QWidget* parent,  const char* name, bool modal, WFlags fl )
 : AnimationDialogData( parent, name, modal, fl )
{
}

AnimationDialog::~AnimationDialog()
{
 // no need to delete child widgets, Qt does it all for us
}

void AnimationDialog::animatePressed()
{
	if (cameras_.size() < 2) return;

	List<Camera>::Iterator it = cameras_.begin();
	Camera last_camera = *it;
	it++;

	for (; it != cameras_.end(); it++)
	{
		if (*it == last_camera) continue;

		Camera camera = last_camera;
		Vector3 diff_viewpoint = (camera.getViewPoint() - (*it).getViewPoint());
		Vector3 diff_up = (camera.getLookUpVector() - (*it).getLookUpVector());
		Vector3 diff_look_at = (camera.getLookAtPosition() - (*it).getLookAtPosition());

		Vector3 max = diff_viewpoint;
		if (diff_look_at.getLength() > max.getLength()) max = diff_look_at;
		Size steps = (Size) (max.getLength() * smoothness->value());
		diff_viewpoint /= steps;
		diff_up /= steps;
		diff_look_at /= steps;

		for (Size i = 0; i < steps; i++)
		{
			camera.setViewPoint(camera.getViewPoint() - diff_viewpoint);
			camera.setLookUpVector(camera.getLookUpVector() - diff_up);
			camera.setLookAtPosition(camera.getLookAtPosition() - diff_look_at);
			((Scene*) Scene::getInstance(0))->setCamera(camera);

			if (export_PNG->isChecked())
			{
				((Scene*) Scene::getInstance(0))->exportPNG();
			}

			if (export_POV->isChecked())
			{
				((Scene*) Scene::getInstance(0))->exportPOVRay();
			}
		}
		
		last_camera = *it;
	}

}

void AnimationDialog::cancelPressed()
{
}

void AnimationDialog::closePressed()
{
	hide();
}

void AnimationDialog::gotoPressed()
{
 	if (entries->selectedItem() == 0) return;

	Position p;
	List<Camera>::Iterator it = cameras_.begin();
	for (p = 0; p < entries->count(); p++)
	{
		if (entries->isSelected(p))
		{
			((Scene*) Scene::getInstance(0))->setCamera(*it);
			return;
		}
		it++;
	}
}

void AnimationDialog::deletePressed()
{
 	if (entries->selectedItem() == 0) return;

	Position p;
	List<Camera>::Iterator it = cameras_.begin();
	for (p = 0; p < entries->count(); p++)
	{
		if (entries->isSelected(p))
		{
			cameras_.erase(it);
			entries->removeItem(p);

			if (entries->count() == 0)
			{
				animate_button->setEnabled(false);
			}

			return;
		}

		it ++;
	}
}

void AnimationDialog::addPressed()
{
	Camera& camera = ((Scene*) Scene::getInstance(0))->getStage()->getCamera();
	String text;
	text += "V: " + vector3ToString(camera.getViewPoint());
	text += " L: " + vector3ToString(camera.getLookAtPosition());
	entries->insertItem(text.c_str());
	entries->update();
	cameras_.push_back(camera);

	animate_button->setEnabled(true);
}

void AnimationDialog::entrySelected()
{
	delete_button->setEnabled(entries->selectedItem() != 0);
	goto_button->setEnabled(entries->selectedItem() != 0);
}

// NAMESPACE
} }
