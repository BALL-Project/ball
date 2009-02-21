// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: materialSettings.C,v 1.11.16.2 2007/05/30 21:23:14 amoll Exp $
// 

#include <BALL/VIEW/DIALOGS/materialSettings.h>
#include <BALL/VIEW/WIDGETS/scene.h>
#include <BALL/VIEW/KERNEL/representation.h>
#include <BALL/VIEW/DIALOGS/modifyRepresentationDialog.h>

#include <QtGui/qlabel.h>
#include <QtGui/qslider.h>

namespace BALL
{
	namespace VIEW
	{

		MaterialSettings::MaterialSettings(QWidget* parent, const char* name, Qt::WFlags fl)
			: QWidget(parent, fl),
				Ui_MaterialSettingsData(),
				PreferencesEntry(),
				current_representation_(0)
		{
			setupUi(this);
			setObjectName(name);
			setINIFileSectionName("MATERIAL_SETTINGS");
			setWidgetStackName("Materials");
			
			setColor(ambient_color_label, ColorRGBA(1.0, 1.0, 1.0));
			setColor(reflectiveness_color_label, ColorRGBA(1.0, 1.0, 1.0));
			setColor(specularity_color_label, ColorRGBA(1.0, 1.0, 1.0));

			// signals and slots connections
			connect( ambient_factor_slider, SIGNAL( valueChanged(int) ), this, SLOT( ambientFactorChanged() ) );
			connect( specularity_factor_slider, SIGNAL( valueChanged(int) ), this, SLOT( specularityFactorChanged() ) );
			connect( reflectiveness_factor_slider, SIGNAL( valueChanged(int) ), this, SLOT( reflectivenessFactorChanged() ) );
			connect( shininess_factor_slider, SIGNAL( valueChanged(int) ), this, SLOT( shininessFactorChanged() ) );

			connect (update_directly_checkBox, SIGNAL(stateChanged(int)), this, SLOT (updateDirectlyBoxChanged()));
		
			connect (radioButton_OpenGL, SIGNAL(clicked()), this, SLOT (rendererChanged()));
			connect (radioButton_POVRay, SIGNAL(clicked()), this, SLOT (rendererChanged()));
			connect (radioButton_RTFact, SIGNAL(clicked()), this, SLOT (rendererChanged()));

			connect( ambient_color_button, SIGNAL( clicked() ), this, SLOT( editAmbientColor() ) );
			connect( specularity_color_button, SIGNAL(clicked()), this, SLOT( editSpecularityColor() ) );
			connect( reflectiveness_color_button, SIGNAL(clicked()), this, SLOT( editReflectivenessColor() ) );

			registerWidgets_();
		}


		void MaterialSettings::apply()
		{
//TODO still missing: use the colors:  ambient_color_button, .. in POVRay and OpenGL 

			//TODO necessary?
			if (Scene::getInstance(0) && Scene::getInstance(0)->getStage())
			{	
				Stage& stage = *Scene::getInstance(0)->getStage();

				//TODO really both??
				if (radioButton_OpenGL->isChecked() || radioButton_POVRay->isChecked())
				{
					stage.setSpecularIntensity(specularity_factor_label->text().toFloat());
					//  ((float)specular_factor_slider->value())  / 100.0);
					//TODO are diffusity and reflectiveness really the same?? 
					stage.setDiffuseIntensity(1. - reflectiveness_factor_label->text().toFloat());
					// ((float)diffuse_factor_slider->value())   / 100.0);
					stage.setAmbientIntensity( ambient_factor_label->text().toFloat());
					//	((float)ambient_factor_slider->value())   / 100.0);
					stage.setShininess(shininess_factor_label->text().toFloat());
					//				(float)shininess_factor_slider->value());
				}
				if (radioButton_OpenGL->isChecked()) // TODO is this correct? names indicate but who knows??
				{
					// shininess uses a logarithmic scale
					glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 2*log(stage.getShininess()+1.1));
					GLfloat values[4];
					values[0] = values[1] = values[2] =  stage.getSpecularIntensity();
					values[3] = 1.0;
					glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  values);
				}
#ifdef ENABLE_RAYTRACING	
				else if (radioButton_RTFact->isChecked())
				{
					// first, decide whether we have been called from the preferences or from the geometric control
					Stage::RaytracingMaterial rt_material;

					rt_material.ambient_color  	 = VIEW::getColor(ambient_color_label);
					rt_material.specular_color 	 = VIEW::getColor(specularity_color_label);
					rt_material.reflective_color = VIEW::getColor(reflectiveness_color_label);

					rt_material.ambient_intensity    = ambient_factor_label->text().toFloat();
					rt_material.specular_intensity   = specularity_factor_label->text().toFloat();
					rt_material.reflective_intensity = reflectiveness_factor_label->text().toFloat();

					// shininess uses a logarithmic scale
					rt_material.shininess          = 2*log(shininess_factor_label->text().toFloat()+1.1);

					if (objectName() == "MaterialSettingsForRepresentation")
					{
						// we have been called from the right mouse button menu of a representation and should *only*
						// apply the values to this one
						if (current_representation_)
							Scene::getInstance(0)->updateRTMaterialForRepresentation(current_representation_, rt_material);
						else
							Log.error() << "MaterialSettings::apply(): Invalid representation selected!" << std::endl;
					}
					else
					{
						// we have been called from the preferences and should set new default values for *all* representations
						rt_material = stage.getRTMaterial() = rt_material;
						Scene::getInstance(0)->updateAllRTMaterials();
					}
				}
#endif
			}
		}

		void MaterialSettings::ambientFactorChanged()
		{ 
			setValues_(*ambient_factor_slider, *ambient_factor_label, 100);
			if (update_directly_checkBox->isChecked())
				apply();
		}

		void MaterialSettings::specularityFactorChanged()
		{
			setValues_(*specularity_factor_slider, *specularity_factor_label, 100);	
			if (update_directly_checkBox->isChecked())
				apply();

		}

		void MaterialSettings::reflectivenessFactorChanged()
		{                                            
			setValues_(*reflectiveness_factor_slider, *reflectiveness_factor_label, 100);	
			if (update_directly_checkBox->isChecked())
				apply();
		}

		void MaterialSettings::shininessFactorChanged()
		{
			setValues_(*shininess_factor_slider, *shininess_factor_label, 1);	
			if (update_directly_checkBox->isChecked())
				apply();
		}
	
		void MaterialSettings::updateDirectlyBoxChanged()
		{
			if (update_directly_checkBox->isChecked())
			{
				apply();
			}
		}

		void MaterialSettings::setValues_(const QSlider& slider, QLabel& label, int divisor)
		{
			String text = String(((float)slider.value()) / divisor);
			
			while (text.has('.') && text.hasSuffix("0"))
			{
				text = text.trimRight("0");
			}

			if (text.hasSuffix(".")) text += "0";
				
			label.setText(text.c_str());
		}
		
		void MaterialSettings::editAmbientColor()
		{
			QColor color = VIEW::chooseColor(ambient_color_label);

			if (update_directly_checkBox->isChecked())
				apply();
		}	
		
		void MaterialSettings::editSpecularityColor()
		{
			QColor color = VIEW::chooseColor(specularity_color_label);

			if (update_directly_checkBox->isChecked())
				apply();
		}
		
		void MaterialSettings::editReflectivenessColor()
		{
			QColor color = VIEW::chooseColor(reflectiveness_color_label);

			if (update_directly_checkBox->isChecked())
				apply();
		}
	
		void MaterialSettings::rendererChanged()
		{
			bool isOpenGL = radioButton_OpenGL->isChecked(); 
			bool isPOVRay = radioButton_POVRay->isChecked();
			bool isRTFact = radioButton_RTFact->isChecked();
			groupBox_ambientIntensity->setDisabled(isOpenGL && (!isPOVRay) && (!isRTFact));
			groupBox_Reflectiveness->setDisabled(isOpenGL && (!isPOVRay) && (!isRTFact));
			if (isOpenGL || isPOVRay)
			{
				update_directly_checkBox->setChecked(false);
			}
			update_directly_checkBox->setDisabled(isOpenGL || isPOVRay);
		}

		
	} // namespace VIEW
} // namespace BALL
