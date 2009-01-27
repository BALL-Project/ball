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
			
			setColor(ambient_color_label, ColorRGBA(1.0, 1.0, 1.0));
			setColor(reflectiveness_color_label, ColorRGBA(1.0, 1.0, 1.0));
			setColor(specularity_color_label, ColorRGBA(1.0, 1.0, 1.0));

			// signals and slots connections
			connect( specular_slider, SIGNAL( valueChanged(int) ), this, SLOT( specularChanged() ) );
			connect( diffuse_slider, SIGNAL( valueChanged(int) ), this, SLOT( diffuseChanged() ) );
			connect( ambient_slider, SIGNAL( valueChanged(int) ), this, SLOT( ambientChanged() ) );
			connect( diffuse_slider, SIGNAL( valueChanged(int) ), this, SLOT( diffuseChanged() ) );
			connect( shininess_slider, SIGNAL( valueChanged(int) ), this, SLOT( shininessChanged() ) );
			
			connect( ambient_factor_slider, SIGNAL( valueChanged(int) ), this, SLOT( ambientFactorChanged() ) );
			connect( specularity_factor_slider, SIGNAL( valueChanged(int) ), this, SLOT( specularityFactorChanged() ) );
			connect( reflectiveness_factor_slider, SIGNAL( valueChanged(int) ), this, SLOT( reflectivenessFactorChanged() ) );
			connect( shininess_factor_slider, SIGNAL( valueChanged(int) ), this, SLOT( shininessFactorChanged() ) );

			connect (update_directly_checkBox, SIGNAL(stateChanged(int)), this, SLOT (updateDirectlyBoxChanged()));
			
			connect( sphere_points_slider, SIGNAL( valueChanged(int) ), this, SLOT( spherePointsChanged() ) );
			connect( tube_points_slider, SIGNAL( valueChanged(int) ), this, SLOT( tubePointsChanged() ) );

			connect( ambient_color_button, SIGNAL( clicked() ), this, SLOT( editAmbientColor() ) );
			connect( specularity_color_button, SIGNAL(clicked()), this, SLOT( editSpecularityColor() ) );
			connect( reflectiveness_color_button, SIGNAL(clicked()), this, SLOT( editReflectivenessColor() ) );

			registerWidgets_();
		}


		void MaterialSettings::apply()
		{
			Stage& stage = *Scene::getInstance(0)->getStage();
			stage.setSpecularIntensity(	((float)specular_slider->value())  / 100.0);
			stage.setDiffuseIntensity(	((float)diffuse_slider->value())   / 100.0);
			stage.setAmbientIntensity(	((float)ambient_slider->value())   / 100.0);
			stage.setShininess(					(float)shininess_slider->value());

			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, stage.getShininess());
			GLfloat values[4];
			values[0] = values[1] = values[2] =  stage.getSpecularIntensity();
			values[3] = 1.0;
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  values);
	
			if (update_directly_checkBox->isChecked())
			{

			// Set the RTFact - settings
			//TODO
			//if (apply_to_selected_radioButton->isChecked())
			//
			//else if (apply_to_all_radioButton->isChecked())
			//{}
			}
		}

		void MaterialSettings::ambientChanged()
		{
			setValues_(*ambient_slider, *ambient_label, 10);
			apply();
		}

		void MaterialSettings::specularChanged()
		{
			setValues_(*specular_slider, *specular_label, 10);	
			apply();
		}

		void MaterialSettings::diffuseChanged()
		{
			setValues_(*diffuse_slider, *diffuse_label, 10);
			apply();
		}

		void MaterialSettings::shininessChanged()
		{
			//TODO shouldn't OpenGL and RTfact shininess factors have the same range?
			setValues_(*shininess_slider, *shininess_label, 10);
			apply();
		}
		
		//              ///////// RTFact ///////////
		void MaterialSettings::ambientFactorChanged()
		{ 
			setValues_(*ambient_factor_slider, *ambient_factor_label, 100);
			if (update_directly_checkBox->isChecked())
			{
				std::cout << "RTFact should change Ambient intensity to "  
					        << ascii(ambient_factor_label->text()) << std::endl;
			}
			apply();

		}
		void MaterialSettings::specularityFactorChanged()
		{
			setValues_(*specularity_factor_slider, *specularity_factor_label, 100);	
			if (update_directly_checkBox->isChecked())
			{
				std::cout << "RTFact should change Specularity factor to " 
									<<  ascii(specularity_factor_label->text()) << std::endl;
			}
			apply();

		}
		void MaterialSettings::reflectivenessFactorChanged()
		{                                            
			setValues_(*reflectiveness_factor_slider, *reflectiveness_factor_label, 100);	
			if (update_directly_checkBox->isChecked())
			{
				std::cout << "RTFact should change Reflectiveness factor to "  
					        <<  ascii(reflectiveness_factor_label->text()) << std::endl;
			}
			apply();

		}
		void MaterialSettings::shininessFactorChanged()
		{
			setValues_(*shininess_factor_slider, *shininess_factor_label, 1);	
			if (update_directly_checkBox->isChecked())
			{
				std::cout << "RTFact should change Shininess factor to "  
					        <<  ascii(shininess_factor_label->text()) << std::endl;
			}
			apply();

		}
	
		void MaterialSettings::updateDirectlyBoxChanged()
		{
			//TODO is this needed??
			if (update_directly_checkBox->isChecked())
			{
				//TODO read out __ALL__ fields	
		/*		ambient_factor_label
				specularity_factor_label	
				reflectiveness_factor_label	
				shininess_factor_label	

				sphere_points_label	
				tube_points_label->text();*/
				ColorRGBA color  = VIEW::getColor(ambient_color_label);
				ColorRGBA color2 = VIEW::getColor(specularity_color_label);
				ColorRGBA color3 = VIEW::getColor(reflectiveness_color_label);
std::cout << " RTFact read __all__ values!" << std::endl;
				apply();
			}
		}

		void MaterialSettings::spherePointsChanged()
		{
			setValues_(*sphere_points_slider, *sphere_points_label, 1);	
			if (update_directly_checkBox->isChecked())
			{	
				int sphere_points = 0;
				try
				{
					sphere_points =  ascii(sphere_points_label->text()).toUnsignedInt();
				}
				catch(...)
				{
					Log.error() << "Invalid sphere resolution!" << std::endl;
					return;
				}

				if (sphere_points < 0)
				{
					Log.error() << "Invalid sphere resolution!" << std::endl;
					return;
				}
				std::cout << "Sphere detail level: " << sphere_points <<std::endl;
				apply();
			}
		}

		void MaterialSettings::tubePointsChanged()
		{	
			setValues_(*tube_points_slider, *tube_points_label, 1);	
			if (update_directly_checkBox->isChecked())
			{	
				int tube_points = 0;

				try
				{
					tube_points =  ascii(tube_points_label->text()).toUnsignedInt();
					if (tube_points < 0)
					{
						Log.error() << "Invalid sphere resolution!" << std::endl;
						return;
					}
				}
				catch(...)
				{
					Log.error() << "Invalid tube resolution!" << std::endl;
					return;
				}	
				std::cout << "Tube points: " << tube_points <<std::endl;
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
			std::cout << "Ambient color is " << color.rgb() << std::endl;
		}	
		
		void MaterialSettings::editSpecularityColor()
		{
			QColor color = VIEW::chooseColor(specularity_color_label);
			std::cout << "Specularity color is " << color.rgb() << std::endl;
		}
		
		void MaterialSettings::editReflectivenessColor()
		{
			QColor color = VIEW::chooseColor(reflectiveness_color_label);
			std::cout << "Reflectiveness color is " << color.rgb() << std::endl;
		}
		
		
	} // namespace VIEW
} // namespace BALL
