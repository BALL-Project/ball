// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: materialSettings.C,v 1.8 2004/10/08 14:49:15 amoll Exp $
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
			apply_(GL_SPECULAR, material_values_[0]);
			apply_(GL_DIFFUSE,  material_values_[1]);
			apply_(GL_AMBIENT,  material_values_[2]);
			glMaterialf(GL_FRONT, GL_SHININESS, material_values_[3]);
		}

		void MaterialSettings::apply_(Index e, float value)
			throw()
		{
			float f[4];
			f[0] = f[1] = f[2] = value;
			f[3] = 1.0;
			glMaterialfv(GL_FRONT, e, f);
		}


		void MaterialSettings::setDefaultValues(bool /*all*/)
			throw()
		{
			material_values_[0] = 0.774;// specular
			material_values_[1] = 0.4; 	// diffuse
			material_values_[2] = 0.25; // ambient
			material_values_[3] = 76.8; // shininess

			setValues_();
		}


		void MaterialSettings::ambientChanged()
		{
			setValues_(*ambient_slider, *ambient_label, 0);
		}

		void MaterialSettings::specularChanged()
		{
			setValues_(*specular_slider, *specular_label, 1);
		}

		void MaterialSettings::diffuseChanged()
		{
			setValues_(*diffuse_slider, *diffuse_label, 2);
		}

		void MaterialSettings::shininessChanged()
		{
			setValues_(*shininess_slider, *shininess_label, 3);
		}

		void MaterialSettings::setValues_(const QSlider& slider, QLabel& label, Position pos)
		{
			String text = String(((float)slider.value()) / 10.0);
			
			while (text.has('.') && text.hasSuffix("0"))
			{
				text = text.trimRight("0");
			}

			if (text.hasSuffix(".")) text += "0";
				
			label.setText(text.c_str());
			material_values_[pos] = ((float)slider.value()) / 10.0;
		}

		void MaterialSettings::setValues_()
		{
			ambient_slider->setValue((Index)(material_values_[0] * 10.0));
			specular_slider->setValue((Index)(material_values_[1] * 10.0));
			diffuse_slider->setValue((Index)(material_values_[2] * 10.0));
			shininess_slider->setValue((Index)(material_values_[3] * 10.0));
			shininessChanged();
			ambientChanged();
			diffuseChanged();
			specularChanged();
		}

	} // namespace VIEW
} // namespace BALL
