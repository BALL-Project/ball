// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#include <BALL/VIEW/DIALOGS/materialSettings.h>
#include <BALL/VIEW/WIDGETS/scene.h>

#include <qlabel.h>
#include <qbuttongroup.h>
#include <qradiobutton.h>
#include <qslider.h>

namespace BALL
{
	namespace VIEW
	{

MaterialSettings::MaterialSettings( QWidget* parent,  const char* name, WFlags fl )
    : MaterialSettingsData( parent, name, fl )
{
	scene_ = (Scene*) parent;
	diffuse_button->setChecked(true);
	setDefaultValues();
}


void MaterialSettings::updateFromStage()
	throw()
{
}


void MaterialSettings::update()
	throw()
{
}


void MaterialSettings::apply()
	throw()
{
	glMaterialfv(GL_FRONT, GL_SPECULAR,  material_values_[0]);
	glMaterialfv(GL_FRONT, GL_DIFFUSE,   material_values_[1]);
	glMaterialfv(GL_FRONT, GL_AMBIENT,   material_values_[2]);
	glMaterialfv(GL_FRONT, GL_SHININESS, &shininess_);
}


void MaterialSettings::setDefaultValues()
	throw()
{
	for (Position i = 0; i < 4; i++)
	{
		material_values_[0][i] = 0.774597;
	}
	for (Position i = 0; i < 4; i++)
	{
		material_values_[1][i] = 0.4;
	}
	for (Position i = 0; i < 4; i++)
	{
		material_values_[2][i] = 0.25;
	}
	shininess_ = 76.8;

	setValues_();
}


void MaterialSettings::redChanged()
{
	setValues_(*red_slider, *red_label, 0);
}

void MaterialSettings::greenChanged()
{
	setValues_(*green_slider, *green_label, 1);
}

void MaterialSettings::blueChanged()
{
	setValues_(*blue_slider, *blue_label, 2);
}

void MaterialSettings::alphaChanged()
{
	setValues_(*alpha_slider, *alpha_label, 3);
}

void MaterialSettings::shininessChanged()
{
	shininess_label->setText(String(shininess_slider->value()).c_str());
	shininess_ = shininess_slider->value();
}

void MaterialSettings::materialSelected(int button_id)
{
		red_slider->setValue((int)(material_values_[button_id][0] * 10.0));
	green_slider->setValue((int)(material_values_[button_id][1] * 10.0));
	 blue_slider->setValue((int)(material_values_[button_id][2] * 10.0));
	alpha_slider->setValue((int)(material_values_[button_id][3] * 10.0));
}
  
void MaterialSettings::setValues_(const QSlider& slider, QLabel& label, Position pos)
{
	String text = String((float)slider.value() / 10.0);
	while (text.has('.') && text.hasSuffix("0"))
	{
		text = text.trimRight("0");
	}

	if (text.hasSuffix(".")) text += "0";
		
	label.setText(text.c_str());
	material_values_[material_group->selectedId()][pos] = slider.value() / 10.0;
}

void MaterialSettings::setValues_()
{
	materialSelected(material_group->selectedId());
	shininess_slider->setValue((int)shininess_);
	shininessChanged();
	redChanged();
	blueChanged();
	greenChanged();
	alphaChanged();
}


// NAMESPACE
} }
