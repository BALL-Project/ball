// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/DIALOGS/lightSettings.h>
#include <BALL/VIEW/WIDGETS/scene.h>

#include <QtGui/QPushButton>
#include <QtGui/QLineEdit> 
#include <QtGui/QLabel>
#include <QtGui/QRadioButton>
#include <QtGui/QSlider>

namespace BALL
{
	namespace VIEW
	{

LightSettings::LightSettings(QWidget* parent, const char* name, Qt::WFlags fl)
  : QWidget(parent, fl),
		Ui_LightSettingsData(),
		PreferencesEntry(),
		Embeddable("BALL::VIEW::LightSettings"),
		ignore_(false),
		current_light_(-1)
{
	setupUi(this);
	setObjectName(name);
	relative->setChecked(true);
	defaultsPressed();
	setWidgetStackName((String)tr("Lighting"));
	setINIFileSectionName("LIGHTING");
	registerWidgetForHelpSystem_(this, "scene.html#lightsources");
	registerWidgets_();
	
	lights_list->setSelectionMode(QAbstractItemView::SingleSelection);	
	// signals and slots connections
	connect( lights_list, SIGNAL( currentItemChanged(QListWidgetItem*, QListWidgetItem*) ), 
	         this,        SLOT( lightSelected(QListWidgetItem*, QListWidgetItem*) ) );
	connect( ambient, SIGNAL( clicked() ), this, SLOT( typeSelected() ) );
	connect( point, SIGNAL( clicked() ), this, SLOT( typeSelected() ) );
	connect( directional, SIGNAL( clicked() ), this, SLOT( typeSelected() ) );
	connect( intensity, SIGNAL( valueChanged(int) ), this, SLOT( intensityChanged() ) );
	connect( max_intensity_factor, SIGNAL(textChanged( const QString& ) ), this, SLOT( intensityMaxChanged(const QString& ) ) );
	connect( color_button, SIGNAL( clicked() ), this, SLOT( colorPressed() ) );
	connect( add_lights_button, SIGNAL( clicked() ), this, SLOT( addLightPressed() ) );
	connect( remove_lights_button, SIGNAL( clicked() ), this, SLOT( removeLightPressed() ) );
	connect( not_relative, SIGNAL( clicked() ), this, SLOT( positionTypeChanged() ) );
	connect( relative, SIGNAL( clicked() ), this, SLOT( positionTypeChanged() ) );
	connect (update_directly_checkBox, SIGNAL(stateChanged(int)), this, SLOT( updateDirectlyBoxChanged()));

	if (parent == 0 || !RTTI::isKindOf<Scene>(*parent)) 
	{
		Log.error() << "LightSettings dialog must be created with a Scene as parent!" << std::endl;
		return;
	}

	scene_ = (dynamic_cast<Scene*>(parent));
	stage_ = scene_->getStage();
	if (stage_ == 0) 
	{
		Log.error() << "LightSettings dialog was created with a Scene as parent, which has no Stage!" << std::endl;
		return;
	}

	registerThis();
}


void LightSettings::updateFromStage()
{
	if (stage_ == 0) return;
	lights_.clear();
	list<LightSource>::const_iterator it = stage_->getLightSources().begin();
	for (; it != stage_->getLightSources().end(); it++)
	{
		lights_.push_back(*it);
	}

	update();

	if (lights_.size() > 0)
	{
		lights_list->setCurrentRow(0);
		current_light_ = 0;
	}
}


void LightSettings::update()
{
	if (!lights_.size()) 
	{
		clearFields_();
		return;
	}

	if (lights_.size() != (Position) lights_list->count())
	{
		clearFields_();
	
		for (Position light_nr = 0; light_nr < lights_.size(); light_nr++)
		{
			lights_list->addItem((String(tr("Light")) + " " + String(light_nr + 1)).c_str());
		}
	}

	if (getCurrentLightNumber_() == -1)
	{
		ignore_ = true;
		lights_list->setItemSelected(lights_list->item(lights_.size() - 1), true);
		ignore_ = false;
		return;
	}
	
	getValues_();
	ignore_ = false;
}


void LightSettings::addLightPressed()
{
	if (lights_.size() >= 8) return;

	saveSettingsToLight_();

	LightSource light;
	light.setIntensity((float) 0.8);
	light.setType(LightSource::POSITIONAL);

	// create a kind of headlight
	// position light 20 space units behind camera position

	light.setRelativeToCamera(true);
	light.setPosition(Vector3(0., 4., -20.));
	light.setDirection(Vector3(0., 0., 1.));
	light.setAttenuation(Vector3(1., 0., 0.));

	lights_.push_back(light);

	update();

	lights_list->setCurrentRow(lights_.size()-1, QItemSelectionModel::Select);
	
	if (update_directly_checkBox->isChecked())
	{
		apply();
		scene_->lightsUpdated(true);
	}
}


void LightSettings::colorPressed()
{
	chooseColor(color_sample);
	
	if (update_directly_checkBox->isChecked())
	{
		apply();
		scene_->lightsUpdated(true);
	}
}


void LightSettings::defaultsPressed()
{
	lights_list->clear();
	lights_.clear();
	max_intensity_factor->setText("500");	
	addLightPressed();
}


// store settings for last selected light
void LightSettings::saveSettingsToLight_()
{
	if (current_light_ == -1 ||
			current_light_ >= (Index)lights_.size()) 
	{
		return;
	}

	LightSource& light = lights_[current_light_];
	light.setColor(getColor(color_sample));

	try
	{
		Vector3 pos = getPosition_();
		Vector3 dir = getDirection_();
		Vector3 att = getAttenuation_();
		bool relative = Ui_LightSettingsData::relative->isChecked();
		light.setRelativeToCamera(relative);

		// position and direction
		
		if (!relative)
		{
			Vector3 diff = dir - pos;
			dir = diff;
		}
		
		light.setPosition(pos);
		light.setDirection(dir);
		light.setAttenuation(att);

		light.setIntensity((float)(intensity->value()) / 100.0);
		
		/////////////////////////////////////////////////////
		// type of light

		if 			(ambient->isChecked())
		{
			light.setType(LightSource::AMBIENT);
		}
		else if (point->isChecked())
		{
			light.setType(LightSource::POSITIONAL);
		}
		else
		{
			light.setType(LightSource::DIRECTIONAL);
		}
	}
	catch (Exception::GeneralException& e)
	{
		Log.error() << "Invalid values in LightSettingsDialog" << std::endl;
		Log.error() << e;
	}
}
	

void LightSettings::lightSelected(QListWidgetItem* /*current_item*/, QListWidgetItem* /*previous_item*/)
{
	if (!ignore_) saveSettingsToLight_();
	current_light_ = lights_list->currentRow();

	getValues_();
}


void LightSettings::removeLightPressed()
{
	Index current = getCurrentLightNumber_();
	if (current == -1) return;

	vector<LightSource>::iterator it = lights_.begin();
	for (Index i = 0; it != lights_.end() && i < current; it++)
	{
		i++;
	}
	lights_.erase(it);

	getValues_(current);
	lights_list->setCurrentRow(current);

	update();
	
	if (update_directly_checkBox->isChecked())
	{
		apply();
		scene_->lightsUpdated(true);
	}
}


void LightSettings::typeSelected()
{
	Position pos = LightSource::AMBIENT;
	if (point->isChecked()) pos = LightSource::POSITIONAL;
	if (directional->isChecked()) pos = LightSource::DIRECTIONAL;

	typeSelected_(pos);

	if (update_directly_checkBox->isChecked())
	{
		apply();
		scene_->lightsUpdated(true);
	}
		
}

void LightSettings::typeSelected_(Position type)
{
	bool is_ambient = (type == LightSource::AMBIENT);
	bool pos_enabled = type != LightSource::DIRECTIONAL && !is_ambient;
	bool is_directional = (type == LightSource::DIRECTIONAL); 

	position_x->setEnabled(pos_enabled);
	position_y->setEnabled(pos_enabled);
	position_z->setEnabled(pos_enabled);

	direction_x->setEnabled(is_directional);
	direction_y->setEnabled(is_directional);
	direction_z->setEnabled(is_directional);
	relative->setEnabled(!is_ambient);
	not_relative->setEnabled(!is_ambient);

	attenuationGroup->setEnabled(pos_enabled);
}

void LightSettings::getValues_(Index current)
{
	if (current == -1)
		current = getCurrentLightNumber_();

	if (current == -1) return;

	setControlsEnabled_(true);

	LightSource& light = lights_[current];
	setColor(color_sample, light.getColor());

	Vector3 pos = light.getPosition();
	Vector3 dir = light.getDirection();
	Vector3 att = light.getAttenuation();

	if (light.isRelativeToCamera())
	{
		relative->setChecked(true);
	}
	else
	{
		not_relative->setChecked(true);
		dir = pos + dir;
	}

	setPosition_(pos);
	setDirection_(dir);
	setAttenuation_(att);
	
	typeSelected_(light.getType());
	
	if (light.getType() == LightSource::AMBIENT) ambient->setChecked(true);
	if (light.getType() == LightSource::POSITIONAL) point->setChecked(true);
	if (light.getType() == LightSource::DIRECTIONAL) directional->setChecked(true);
	intensity->setValue((Index)(light.getIntensity() * 100.0));
}


void LightSettings::clearFields_()
{
	ignore_ = true;
	lights_list->clear();
	ignore_ = false;

	position_x->clear();
	position_y->clear();
	position_z->clear();
	direction_x->clear();
	direction_y->clear();
	direction_z->clear();
	attenuation_p_1->clear();
	attenuation_p_2->clear();
	attenuation_p_3->clear();
	setControlsEnabled_(false);
}


void LightSettings::setControlsEnabled_(bool state)
{
	remove_lights_button->setEnabled(state);
	position_x->setEnabled(state);
	position_y->setEnabled(state);
	position_z->setEnabled(state);
	direction_x->setEnabled(state);
	direction_y->setEnabled(state);
	direction_z->setEnabled(state);
	intensity->setEnabled(state);
	remove_lights_button->setEnabled(state);
	relative->setEnabled(state);
	not_relative->setEnabled(state);
	color_button->setEnabled(state);
	point->setEnabled(state);
	directional->setEnabled(state);
	attenuation_p_1->setEnabled(state);
	attenuation_p_2->setEnabled(state);
	attenuation_p_3->setEnabled(state);

	ambient->setEnabled(state);
}

void LightSettings::apply()
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
	
	if (update_directly_checkBox->isChecked())
	{
		apply();
		scene_->lightsUpdated(true);
	}

}

void LightSettings::intensityMaxChanged(const QString& text)
{
	if (text.toFloat()>0)
		intensity->setMaximum(text.toFloat());
	
	if (update_directly_checkBox->isChecked())
	{
		apply();
		scene_->lightsUpdated(true);
	}
}

void LightSettings::restoreDefaultValues(bool /*all*/)
{
	defaultsPressed();
	setColor(color_sample, ColorRGBA(255,255,255));
	lights_list->setCurrentItem(0);
}

void LightSettings::positionTypeChanged()
{
	if (getCurrentLightNumber_() == -1 || ignore_) 
	{
		return;
	}

	try
	{
		Vector3 pos = getPosition_();
		Vector3 dir = getDirection_();

		const Vector3& vp = stage_->getCamera().getViewPoint();

		if (relative->isChecked())
		{
			Vector3 diff = dir - pos;
			dir = stage_->calculateRelativeCoordinates(diff);

			pos -= vp;
			pos = stage_->calculateRelativeCoordinates(pos);
		}
		else
		{
			pos = stage_->calculateAbsoluteCoordinates(pos) + vp;
			dir = stage_->calculateAbsoluteCoordinates(dir) + pos;
		}

		setPosition_(pos);
		setDirection_(dir);
	}
	catch(...)
	{
		BALLVIEW_DEBUG
	}
	
	if (update_directly_checkBox->isChecked())
	{
		apply();
		scene_->lightsUpdated(true);
	}
}	

void LightSettings::updateDirectlyBoxChanged()
{
	if (update_directly_checkBox->isChecked())
	{
		apply();
		scene_->lightsUpdated(true);
	}
}


void LightSettings::setPosition_(const Vector3& v)
{
	position_x->setText(createFloatString(v.x, 4).c_str());
	position_y->setText(createFloatString(v.y, 4).c_str());
	position_z->setText(createFloatString(v.z, 4).c_str());
}

void LightSettings::setDirection_(const Vector3& v)
{
	direction_x->setText(createFloatString(v.x, 4).c_str());
	direction_y->setText(createFloatString(v.y, 4).c_str());
	direction_z->setText(createFloatString(v.z, 4).c_str());
}

void LightSettings::setAttenuation_(const Vector3& a)
{
	 attenuation_p_1->setText(createFloatString(a.x, 4).c_str());
	 attenuation_p_2->setText(createFloatString(a.y, 4).c_str());
	 attenuation_p_3->setText(createFloatString(a.z, 4).c_str());
}

Vector3 LightSettings::getPosition_() 
	throw(Exception::InvalidFormat)
{
	return Vector3(position_x->text().toFloat(),
				 			   position_y->text().toFloat(),
			  				 position_z->text().toFloat());
}

Vector3 LightSettings::getDirection_() 
	throw(Exception::InvalidFormat)
{
	return Vector3(direction_x->text().toFloat(),
				 			   direction_y->text().toFloat(),
			  				 direction_z->text().toFloat());
}

Vector3 LightSettings::getAttenuation_() 
	throw(Exception::InvalidFormat)
{
	return Vector3(attenuation_p_1->text().toFloat(),
				 			   attenuation_p_2->text().toFloat(),
			  				 attenuation_p_3->text().toFloat());
}


Index LightSettings::getCurrentLightNumber_() const
{
	QList<QListWidgetItem*> items = lights_list->selectedItems();

	if (items.size() == 0) 
		return -1;

	return lights_list->currentRow();
}

void LightSettings::restoreValues(bool)
{
	updateFromStage();
}
		
} } // NAMESPACE
