// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: materialSettings.C,v 1.11.16.2 2007/05/30 21:23:14 amoll Exp $
// 

#include <BALL/VIEW/DIALOGS/materialSettings.h>
#include <BALL/VIEW/WIDGETS/scene.h>

#include <QtGui/qlabel.h>
#include <QtGui/qslider.h>

namespace BALL
{
	namespace VIEW
	{

		MaterialSettings::MaterialSettings(QWidget* parent, const char* name, Qt::WFlags fl)
			: QWidget(parent, fl),
				Ui_MaterialSettingsData(),
				PreferencesEntry()
		{
			setupUi(this);
			setObjectName(name);
			setINIFileSectionName("MATERIAL_SETTINGS");
			setWidgetStackName("Materials");
			
			// signals and slots connections
			connect( specular_slider, SIGNAL( valueChanged(int) ), this, SLOT( specularChanged() ) );
			connect( diffuse_slider, SIGNAL( valueChanged(int) ), this, SLOT( diffuseChanged() ) );
			connect( ambient_slider, SIGNAL( valueChanged(int) ), this, SLOT( ambientChanged() ) );
			connect( diffuse_slider, SIGNAL( valueChanged(int) ), this, SLOT( diffuseChanged() ) );
			connect( shininess_slider, SIGNAL( valueChanged(int) ), this, SLOT( shininessChanged() ) );

			registerWidgets_();
		}


		void MaterialSettings::apply()
			throw()
		{
			Stage& stage = *Scene::getInstance(0)->getStage();
			stage.setSpecularIntensity(	((float)specular_slider->value())  / 10.0);
			stage.setDiffuseIntensity(	((float)diffuse_slider->value())   / 10.0);
			stage.setAmbientIntensity(	((float)ambient_slider->value())   / 10.0);
			stage.setShininess(					((float)shininess_slider->value()) / 10.0);

			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, stage.getShininess());
			GLfloat values[4];
			values[0] = values[1] = values[2] =  stage.getSpecularIntensity();
			values[3] = 1.0;
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  values);
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
