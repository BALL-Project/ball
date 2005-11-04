// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: materialSettings.C,v 1.10.4.2 2005/11/04 15:25:51 amoll Exp $
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

			specular_slider->setValue((Index)(0.774 * 10.0));
			diffuse_slider->setValue((Index)(0.4   	* 10.0));
			ambient_slider->setValue((Index)(0.25 	* 10.0));
			shininess_slider->setValue((Index)(76.8 * 10.0));

			registerObject_(specular_slider);
			registerObject_(diffuse_slider);
			registerObject_(ambient_slider);
			registerObject_(shininess_slider);
			setWidgetStackName("Materials");
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
			GLfloat values[4];
			values[3] = 1.0;

			values[0] = values[1] = values[2] =  stage.getSpecularIntensity();
			glMaterialfv(GL_FRONT, GL_SPECULAR,  values);
			
			values[0] = values[1] = values[2] = stage.getDiffuseIntensity();
			glMaterialfv(GL_FRONT, GL_DIFFUSE,   values);
			
			values[0] = values[1] = values[2] = stage.getAmbientIntensity();
 			glMaterialfv(GL_FRONT, GL_AMBIENT,   values);
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
