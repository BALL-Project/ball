// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: lightSettings.C,v 1.14 2005/02/10 23:21:54 amoll Exp $
//

#include <BALL/VIEW/DIALOGS/lightSettings.h>
#include <BALL/VIEW/WIDGETS/scene.h>

#include <qpushbutton.h>
#include <qlineedit.h> 
#include <qlistbox.h>
#include <qcolordialog.h>
#include <qlabel.h>
#include <qbuttongroup.h>
#include <qradiobutton.h>
#include <qslider.h>

namespace BALL
{
	namespace VIEW
	{

LightSettings::LightSettings(QWidget* parent,  const char* name, WFlags fl)
  : LightSettingsData(parent, name, fl),
		PreferencesEntry()
{
	stage_ = ((Scene*) parent)->getStage();
	if (stage_ == 0) return;
	scene_ = (Scene*) parent;
	current_light_ = -1;
	
	updateFromStage();
	insertEntry(this, "Lighting");
}


void LightSettings::updateFromStage()
	throw()
{
	lights_.clear();
	List<LightSource>::ConstIterator it = stage_->getLightSources().begin();
	for (; it != stage_->getLightSources().end(); it++)
	{
		lights_.push_back(LightSource(*it));
	}

	clearFields_();
	update();
	lights_list->setCurrentItem(lights_.size() -1 );
}


void LightSettings::update()
	throw()
{
	current_light_ = lights_list->currentItem();
	clearFields_();
	if (lights_.size() == 0)
	{
		current_light_ = -1;
		return;
	} 

	if (current_light_ >= (Index)lights_.size()) 
	{
		current_light_ = -1;
	}

	for (Index light_nr = 0; light_nr < (Index)lights_.size(); light_nr++)
	{
		lights_list->insertItem(QString((String("Light ") + String(light_nr + 1)).c_str()));
	}
	
	getValues_();
}


void LightSettings::addLightPressed()
{
	saveSettingsToLight_();
	LightSource light;
	lights_.push_back(light);
	lights_list->insertItem(QString((String("Light ") + String(lights_.size()+ 1)).c_str()));
	current_light_ = lights_.size()-1;
	lights_list->setCurrentItem(current_light_);
	update();
}


void LightSettings::colorPressed()
{
	color_sample->setBackgroundColor(QColorDialog::getColor(color_sample->backgroundColor()));
	QWidget::update();
}


void LightSettings::defaultsPressed()
{
	clearFields_();
	lights_.clear();
	const Camera& camera = stage_->getCamera();
	LightSource light;
	light.setType(LightSource::POSITIONAL);

	// position light 20 space units behind camera position
	Vector3 pos = camera.getViewVector();
	pos.normalize();
	pos *= -20;
	pos += camera.getViewPoint();
	
	// create a kind of headlight
	Vector3 up = camera.getLookUpVector();
	up.normalize();
	up *= 4;
 	pos += up;
	
	light.setPosition(pos);
	light.setDirection(camera.getLookAtPosition());

	lights_.push_back(light);
	current_light_ = -1;
	update();
}


// store settings for last selected light
void LightSettings::saveSettingsToLight_()
	throw()
{
	if (current_light_ == -1)
	{
		return;
	}

	LightSource& light = lights_[current_light_];
	light.setColor(color_sample->backgroundColor());

	if (relative->isChecked())
	{
		light.setRelativeToCamera(true);
	}
	else
	{
		light.setRelativeToCamera(false);
	}

	try
	{
		// ---------------- ALL ------------------------
		light.setIntensity((float)(intensity->value()) / 100);

		if (light_type->selected() != (QButton*) ambient)
		{
			// ------- POINT && DIRECTIONAL --------------
			Vector3 v3;
			v3.set(String(direction_x->text().ascii()).toFloat(),
						 String(direction_y->text().ascii()).toFloat(),
						 String(direction_z->text().ascii()).toFloat());
			light.setDirection(v3);

			v3.set(String(position_x->text().ascii()).toFloat(),
						 String(position_y->text().ascii()).toFloat(),
						 String(position_z->text().ascii()).toFloat());
			light.setPosition(v3);

			if (light_type->selected() == (QButton*) point)
			{
				light.setType(LightSource::POSITIONAL);
			}
			else
			{
				light.setType(LightSource::DIRECTIONAL);
			}
		}
		else
		{
			light.setType(LightSource::AMBIENT);
		}
	}
	catch (Exception::GeneralException e)
	{
		Log.error() << "Invalid values in LightSettingsDialog" << std::endl;
		Log.error() << e;
	}
}
	

void LightSettings::lightSelected()
{
	if (lights_list->currentItem() == -1) return;	
	saveSettingsToLight_();
	current_light_ = lights_list->currentItem();
	getValues_();
}


void LightSettings::removeLightPressed()
{
	current_light_ = lights_list->currentItem();
	if (current_light_ > (Index)lights_.size())
	{
		Log.error() << "Trying to delete a not existing light" << std::endl;
		return;
	}
	
	vector<LightSource>::iterator it = lights_.begin();
	for (Index i=0; it != lights_.end() && i < current_light_; it++)
	{
		i++;
	};
	lights_.erase(it);

	current_light_ = -1;
	clearFields_();
	update();
}


void LightSettings::typeSelected()
{
	if (light_type->selected() == (QButton*) ambient)
	{
		position_x->setEnabled(false);
		position_y->setEnabled(false);
		position_z->setEnabled(false);
		direction_x->setEnabled(false);
		direction_y->setEnabled(false);
		direction_z->setEnabled(false);
		relative_to_camera->setEnabled(false);
	}
	else 
	{
		position_x->setEnabled(true);
		position_y->setEnabled(true);
		position_z->setEnabled(true);
		direction_x->setEnabled(true);
		direction_y->setEnabled(true);
		direction_z->setEnabled(true);

		if (light_type->selected() != (QButton*) ambient)
		{
			relative_to_camera->setEnabled(true);
		}	
		else
		{
			relative_to_camera->setEnabled(false);
		}
	}
}


void LightSettings::getValues_()
	throw()
{
	if (current_light_ == -1) return;

	if ((Position)current_light_ >= lights_.size()) 
	{
		Log.error() << "Wrong light in " << __FILE__ << "  " << __LINE__ << " : " 
								<< current_light_ << std::endl;
		return;
	}

	LightSource* light_source = &lights_[current_light_];

	lights_list->setCurrentItem(current_light_);
	lights_list->setSelected(current_light_, true);

	color_sample->setBackgroundColor(light_source->getColor().getQColor());

	position_x->setText(String(light_source->getPosition().x).c_str());
	position_y->setText(String(light_source->getPosition().y).c_str());
	position_z->setText(String(light_source->getPosition().z).c_str());
	direction_x->setText(String(light_source->getDirection().x).c_str());
	direction_y->setText(String(light_source->getDirection().y).c_str());
	direction_z->setText(String(light_source->getDirection().z).c_str());

	if (light_source->getType() == LightSource::AMBIENT)
	{
		position_x->setEnabled(false);
		position_y->setEnabled(false);
		position_z->setEnabled(false);
		direction_x->setEnabled(false);
		direction_y->setEnabled(false);
		direction_z->setEnabled(false);
		relative_to_camera->setEnabled(false);
	}
	else
	{
		position_x->setEnabled(true);
		position_y->setEnabled(true);
		position_z->setEnabled(true);
		direction_x->setEnabled(true);
		direction_y->setEnabled(true);
		direction_z->setEnabled(true);
		relative_to_camera->setEnabled(true);
	}
	
	if (light_source->isRelativeToCamera())
	{
		relative->setChecked(true);
	}
	else
	{
		not_relative->setChecked(true);
	}

	light_type->setButton(light_source->getType());
	intensity->setValue((unsigned int)(light_source->getIntensity() * 100));
	intensity->setEnabled(true);
	remove_lights_button->setEnabled(true);
}


void LightSettings::clearFields_()
	throw()
{
	lights_list->clear();
	position_x->clear();
	position_y->clear();
	position_z->clear();
	direction_x->clear();
	direction_y->clear();
	direction_z->clear();
	remove_lights_button->setEnabled(false);
	position_x->setEnabled(false);
	position_y->setEnabled(false);
	position_z->setEnabled(false);
	direction_x->setEnabled(false);
	direction_y->setEnabled(false);
	direction_z->setEnabled(false);
	intensity->setEnabled(false);
	intensity->setValue(0);
	remove_lights_button->setEnabled(false);
	relative_to_camera->setButton(-1);
}


void LightSettings::apply()
	throw()
{
	saveSettingsToLight_();
	stage_->clearLightSources();
	for (Position p = 0; p < lights_.size(); p++)
	{
		stage_->addLightSource(lights_[p]);
	}
}


void LightSettings::intensityChanged()
{
	intensity_label->setText(String(intensity->value()).c_str());
}


void LightSettings::setDefaultValues(bool /*all*/)
	throw()
{
	defaultsPressed();
	color_sample->setBackgroundColor(white);
	lights_list->setCurrentItem(0);
}

// NAMESPACE
} }
