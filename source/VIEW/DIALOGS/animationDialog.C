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
#include <qapplication.h>

namespace BALL
{
	namespace VIEW
	{

AnimationDialog::AnimationDialog( QWidget* parent,  const char* name, bool modal, WFlags fl )
 : AnimationDialogData( parent, name, modal, fl ),
	 animation_thread_(0)
{
}

AnimationDialog::~AnimationDialog()
{
	if (animation_thread_ != 0) delete animation_thread_;
 // no need to delete child widgets, Qt does it all for us
}

void AnimationDialog::animatePressed()
{
#ifdef BALL_QT_HAS_THREADS
	if (animation_thread_ != 0) delete animation_thread_;
	animation_thread_ = new AnimationThread();
	animation_thread_->setAnimationDialog(this);
	animation_thread_->start();
	return;
#endif
	
	animate_();
}

void AnimationDialog::animate_()
{
	if (cameras_.size() < 2) return;

	stop_ = false;
	cancel_button->setEnabled(true);
	animate_button->setEnabled(false);

	List<Camera>::Iterator it = cameras_.begin();
	Camera last_camera = *it;
	it++;

	for (; it != cameras_.end(); it++)
	{
//	 	qApp->wakeUpGuiThread();
// 		qApp->processEvents(500);

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

		for (Size i = 0; i < steps && !stop_; i++)
		{
			camera.setViewPoint(camera.getViewPoint() - diff_viewpoint);
			camera.setLookUpVector(camera.getLookUpVector() - diff_up);
			camera.setLookAtPosition(camera.getLookAtPosition() - diff_look_at);
			Scene* scene = ((Scene*) Scene::getInstance(0));

			Scene::SceneSetCameraEvent* e = new Scene::SceneSetCameraEvent();
			e->camera = camera;
			qApp->postEvent(scene, e);

			if (export_PNG->isChecked())
			{
				Scene::SceneExportPNGEvent* e = new Scene::SceneExportPNGEvent();
				qApp->postEvent(scene, e);
			}

			if (export_POV->isChecked())
			{
				Scene::SceneExportPOVEvent* e = new Scene::SceneExportPOVEvent();
				qApp->postEvent(scene, e);
			}
		}
		
		last_camera = *it;
	}

	cancel_button->setEnabled(false);
	animate_button->setEnabled(true);
}

void AnimationDialog::cancelPressed()
{
	stop_ = true;
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
