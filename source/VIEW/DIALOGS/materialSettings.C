// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: materialSettings.C,v 1.10 2005/02/06 20:57:08 oliver Exp $
// 

#include <BALL/VIEW/DIALOGS/materialSettings.h>
#include <BALL/VIEW/WIDGETS/scene.h>

#include <qlabel.h>
#include <qslider.h>

namespace BALL
{
	namespace VIEW
	{

		MaterialSettings::MaterialSettings(QWidget* parent, const char* name, WFlags fl)
			: MaterialSettingsData( parent, name, fl ),
				PreferencesEntry()
		{
			setINIFileSectionName("MATERIAL_SETTINGS");
			setDefaultValues(true);
			registerObject_(specular_slider);
			registerObject_(diffuse_slider);
			registerObject_(ambient_slider);
			registerObject_(shininess_slider);
			insertEntry(this, "Materials");
		}


		void MaterialSettings::apply()
			throw()
		{
			Stage& stage = *Scene::getInstance(0)->getStage();
			stage.setSpecularIntensity(	((float)specular_slider->value())  / 10.0);
			stage.setDiffuseIntensity(	((float)diffuse_slider->value())   / 10.0);
			stage.setAmbientIntensity(	((float)ambient_slider->value())   / 10.0);
			stage.setShininess(					((float)shininess_slider->value()) / 10.0);
			glMaterialf(GL_FRONT, GL_SHININESS, stage.getShininess());
			glMaterialf(GL_FRONT, GL_SPECULAR,  stage.getSpecularIntensity());
			glMaterialf(GL_FRONT, GL_DIFFUSE,   stage.getDiffuseIntensity());
			glMaterialf(GL_FRONT, GL_AMBIENT,   stage.getAmbientIntensity());

		}

		void MaterialSettings::setDefaultValues(bool /*all*/)
			throw()
		{
			specular_slider->setValue((Index)(0.774 * 10.0));
			diffuse_slider->setValue((Index)(0.4   	* 10.0));
			ambient_slider->setValue((Index)(0.25 	* 10.0));
			shininess_slider->setValue((Index)(76.8 * 10.0));
		}


		void MaterialSettings::ambientChanged()
		{
			setValues_(*ambient_slider, *ambient_label);
		}

		void MaterialSettings::specularChanged()
		{
			setValues_(*specular_slider, *specular_label);
		}

		void MaterialSettings::diffuseChanged()
		{
			setValues_(*diffuse_slider, *diffuse_label);
		}

		void MaterialSettings::shininessChanged()
		{
			setValues_(*shininess_slider, *shininess_label);
		}

		void MaterialSettings::setValues_(const QSlider& slider, QLabel& label)
		{
			String text = String(((float)slider.value()) / 10.0);
			
			while (text.has('.') && text.hasSuffix("0"))
			{
				text = text.trimRight("0");
			}

			if (text.hasSuffix(".")) text += "0";
				
			label.setText(text.c_str());
		}

	} // namespace VIEW
} // namespace BALL
