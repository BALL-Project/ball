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
	const ColorRGBA& color = stage_->getBackgroundColor();
	QColor qcolor(color.getRed(), color.getGreen(), color.getBlue());
	color_sample->setBackgroundColor(qcolor);

	coordinate_button->setChecked(stage_->coordinateSystemEnabled());
}


void StageSettings::apply()
	throw()
{
	const QColor& qcolor = color_sample->backgroundColor();
	stage_->setBackgroundColor(ColorRGBA((float)qcolor.red() / 255.0,
													 						 (float)qcolor.green() / 255.0,
																			 (float)qcolor.blue() / 255.0));

	stage_->showCoordinateSystem(coordinate_button->isChecked());
}


void StageSettings::setDefaultValues()
	throw()
{
	color_sample->setBackgroundColor(black);
	coordinate_button->setChecked(false);
}

// NAMESPACE
} }
