// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#include <BALL/VIEW/DIALOGS/stageSettings.h>
#include <BALL/VIEW/WIDGETS/scene.h>
#include <BALL/VIEW/KERNEL/stage.h>

#include <qpushbutton.h>
#include <qcolordialog.h>
#include <qlabel.h>
#include <qcheckbox.h>
#include <qslider.h>

namespace BALL
{
	namespace VIEW
	{

StageSettings::StageSettings( QWidget* parent,  const char* name, WFlags fl )
    : StageSettingsData( parent, name, fl )
{
	stage_ = ((Scene*) parent)->getStage();
	if (stage_ == 0) return;
	scene_ = (Scene*) parent;
	
	updateFromStage();
}


void StageSettings::colorPressed()
{
	color_sample->setBackgroundColor(QColorDialog::getColor(color_sample->backgroundColor()));
	QWidget::update();
}


void StageSettings::updateFromStage()
	throw()
{
	color_sample->setBackgroundColor(stage_->getBackgroundColor().getQColor());
	coordinate_button->setChecked(stage_->coordinateSystemEnabled());

	slider_->setValue((int) Scene::getMouseSensitivity() - 1);
	wheel_slider_->setValue((int) Scene::getMouseWheelSensitivity() - 1);
}


void StageSettings::apply()
	throw()
{
	stage_->setBackgroundColor(color_sample->backgroundColor());
	stage_->showCoordinateSystem(coordinate_button->isChecked());

	Scene::setMouseSensitivity(slider_->value() + 1);
	Scene::setMouseWheelSensitivity(wheel_slider_->value() + 1);
}


void StageSettings::setDefaultValues()
	throw()
{
	color_sample->setBackgroundColor(black);
	coordinate_button->setChecked(false);

	slider_->setValue(5);
}

// NAMESPACE
} }
