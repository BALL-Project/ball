// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#include <BALL/VIEW/DIALOGS/animationDialog.h>
#include <BALL/VIEW/WIDGETS/scene.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/stage.h>

#include <qpushbutton.h>
#include <qlistbox.h>

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

Log.error() << "#~~#   1 "    << __FILE__ << "  " << __LINE__<< std::endl;
	animate_button->setEnabled(true);
}

void AnimationDialog::entrySelected()
{
	if (entries->selectedItem() == 0)
	{
		delete_button->setEnabled(false);
		goto_button->setEnabled(false);
	}
}

// NAMESPACE
} }
