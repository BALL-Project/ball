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

	eye_distance_slider->setValue((int) (stage_->getEyeDistance() * 10.0));
	focal_distance_slider->setValue((int) (stage_->getFocalDistance()));
	eyeDistanceChanged();
	focalDistanceChanged();
}


void StageSettings::apply()
	throw()
{
	stage_->setBackgroundColor(color_sample->backgroundColor());
	stage_->showCoordinateSystem(coordinate_button->isChecked());

	Scene::setMouseSensitivity(slider_->value() + 1);
	Scene::setMouseWheelSensitivity(wheel_slider_->value() + 1);

	stage_->setEyeDistance((float)(eye_distance_slider->value() / 10.0));
	stage_->setFocalDistance((float)(eye_distance_slider->value()));
}


void StageSettings::setDefaultValues()
	throw()
{
	color_sample->setBackgroundColor(black);
	coordinate_button->setChecked(false);

	slider_->setValue(5);

	eye_distance_slider->setValue(20);
	focal_distance_slider->setValue(40);
}

void StageSettings::eyeDistanceChanged()
{
	if (eye_distance_slider->value() == 0)
	{
		eye_distance_label->setText("0");
		return;
	}

	String text((float)(eye_distance_slider->value()) / 10.0);
	while(text[text.size() -1] == '.' || text[text.size() -1] == '0')
	{
		text.truncate(text.size() - 1);
	}
	eye_distance_label->setText(text.c_str());
}

void StageSettings::focalDistanceChanged()
{
	if (focal_distance_slider->value() == 0)
	{
		focal_distance_label->setText("0");
		return;
	}

	String text(focal_distance_slider->value());
	while(text.has('.'))
	{
		text.truncate(text.size() - 1);
	}
	focal_distance_label->setText(text.c_str());
}


// NAMESPACE
} }
