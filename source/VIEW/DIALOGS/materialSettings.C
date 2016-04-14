// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/DIALOGS/materialSettings.h>
#include <BALL/VIEW/WIDGETS/scene.h>
#include <BALL/VIEW/KERNEL/representation.h>
#include <BALL/VIEW/DIALOGS/modifyRepresentationDialog.h>
#include <BALL/CONCEPT/property.h>

#include <QtWidgets/QLabel>
#include <QtWidgets/QSlider>

namespace BALL
{
	namespace VIEW
	{

		MaterialSettings::MaterialSettings(QWidget* parent, const char* name, Qt::WindowFlags fl)
			: QWidget(parent, fl),
				Ui_MaterialSettingsData(),
				PreferencesEntry(),
				current_representation_(0)
		{
			setupUi(this);
			setObjectName(name);
			setINIFileSectionName("MATERIAL_SETTINGS");
			setWidgetStackName((String)tr("Materials"));
			
			setColor(ambient_color_label, ColorRGBA(1.0, 1.0, 1.0));
			setColor(reflectiveness_color_label, ColorRGBA(1.0, 1.0, 1.0));
			setColor(specularity_color_label, ColorRGBA(1.0, 1.0, 1.0));

			// signals and slots connections
			connect( ambient_factor_slider, SIGNAL( valueChanged(int) ), this, SLOT( ambientFactorChanged() ) );
			connect( specularity_factor_slider, SIGNAL( valueChanged(int) ), this, SLOT( specularityFactorChanged() ) );
			connect( reflectiveness_factor_slider, SIGNAL( valueChanged(int) ), this, SLOT( reflectivenessFactorChanged() ) );
			connect( shininess_factor_slider, SIGNAL( valueChanged(int) ), this, SLOT( shininessFactorChanged() ) );
			connect( transparency_factor_slider, SIGNAL( valueChanged(int) ), this, SLOT( transparencyFactorChanged() ) );

			connect (update_directly_checkBox, SIGNAL(stateChanged(int)), this, SLOT (updateDirectlyBoxChanged()));
		
			connect( ambient_color_button, SIGNAL( clicked() ), this, SLOT( editAmbientColor() ) );
			connect( specularity_color_button, SIGNAL(clicked()), this, SLOT( editSpecularityColor() ) );
			connect( reflectiveness_color_button, SIGNAL(clicked()), this, SLOT( editReflectivenessColor() ) );

			registerWidgets_();
		}


		void MaterialSettings::apply()
		{
//TODO still missing: use the colors (ambient_color_button, ..) in POVRay and OpenGL 
//TODO what should happen, when this dialog is called for a specific representation in POVRay and OpenGL?
//TODO So far OpenGL and POVRay do not make any use of transparency!
			if (Scene::getInstance(0) && Scene::getInstance(0)->getStage())
			{
				Stage& stage = *Scene::getInstance(0)->getStage();

					// first, decide whether we have been called from the preferences or from the geometric control
					Stage::Material material;

					material.ambient_color    = VIEW::getColor(ambient_color_label);
					material.specular_color   = VIEW::getColor(specularity_color_label);
					material.reflective_color = VIEW::getColor(reflectiveness_color_label);

					material.ambient_intensity    = ambient_factor_label->text().toFloat();
					material.specular_intensity   = specularity_factor_label->text().toFloat();
					material.reflective_intensity = reflectiveness_factor_label->text().toFloat();

					material.shininess    = std::max(shininess_factor_label->text().toFloat(), 0.1f);
					material.transparency = transparency_factor_label->text().toFloat();

					if (objectName() == "MaterialSettingsForRepresentation")
					{
						// we have been called from the right mouse button menu of a representation and should *only*
						// apply the values to this one
						if (current_representation_)
						{
							Scene::getInstance(0)->updateMaterialForRepresentation(current_representation_, material);
						}
						else
							Log.error() << "MaterialSettings::apply(): Invalid representation selected!" << std::endl;
					}
					else
					{
						// we have been called from the preferences and should set new default values for *all* representations
						stage.getMaterial() = material;
						Scene::getInstance(0)->updateAllMaterials();
					}
				}
		}

		void MaterialSettings::setCurrentRepresentation(Representation* representation)
		{
			current_representation_ = representation;

			// update the sliders
			Stage::Material material;

			if (representation->hasProperty("Rendering::Material"))
			{
				NamedProperty mat_property = representation->getProperty("Rendering::Material");
				boost::shared_ptr<PersistentObject> mat_ptr = mat_property.getSmartObject();
				material = *(dynamic_cast<Stage::Material*>(mat_ptr.get()));
			}
			else
			{
				Stage* stage = Scene::getInstance(0)->getStage();
				material = stage->getMaterial();
			}

			// remember the update-directly value
			bool do_update = update_directly_checkBox->isChecked();
			// and disable it for now to prevent unnecessary raytracing calls
			update_directly_checkBox->setChecked(false);

			// now set the values
			setColor(ambient_color_label, material.ambient_color);
			setColor(reflectiveness_color_label, material.reflective_color);
			setColor(specularity_color_label, material.specular_color);

			setLabel_(*ambient_factor_label, material.ambient_intensity);
			ambient_factor_slider->setValue(material.ambient_intensity*(float)ambient_factor_slider->maximum());

			setLabel_(*specularity_factor_label, material.specular_intensity);
			specularity_factor_slider->setValue(material.specular_intensity*(float)specularity_factor_slider->maximum());

			setLabel_(*reflectiveness_factor_label, material.reflective_intensity);
			reflectiveness_factor_slider->setValue(material.reflective_intensity*(float)reflectiveness_factor_slider->maximum());

			setLabel_(*shininess_factor_label, material.shininess);
			shininess_factor_slider->setValue(sqrt(material.shininess*(float)shininess_factor_slider->maximum()));

			setLabel_(*transparency_factor_label, material.transparency);
			transparency_factor_slider->setValue(material.transparency);

			update_directly_checkBox->setChecked(do_update);
		}

		void MaterialSettings::updateDefaultMaterialsFromStage()
		{
			Stage* stage = Scene::getInstance(0)->getStage();
			const Stage::Material& material = stage->getMaterial();

			// now set the values
			setColor(ambient_color_label, material.ambient_color);
			setColor(reflectiveness_color_label, material.reflective_color);
			setColor(specularity_color_label, material.specular_color);

			ambient_factor_slider->setValue(material.ambient_intensity);
			setValues_(*ambient_factor_slider, *ambient_factor_label, 1);

			specularity_factor_slider->setValue(material.specular_intensity);
			setValues_(*specularity_factor_slider, *specularity_factor_label, 1);

			reflectiveness_factor_slider->setValue(material.reflective_intensity);
			setValues_(*reflectiveness_factor_slider, *reflectiveness_factor_label, 1);

			shininess_factor_slider->setValue(material.shininess);
			setValues_(*shininess_factor_slider, *shininess_factor_label, 1);	

			transparency_factor_slider->setValue(material.transparency*100);
			setValues_(*transparency_factor_slider, *transparency_factor_label, 1);

			if (update_directly_checkBox->isChecked())
				apply();
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
			//setValues_(*shininess_factor_slider, *shininess_factor_label, 1);	
			setQuadraticValues_(*shininess_factor_slider, *shininess_factor_label, shininess_factor_slider->maximum());	
			if (update_directly_checkBox->isChecked())
				apply();
		}
		
		void MaterialSettings::transparencyFactorChanged()
		{
			String text = String((int)transparency_factor_slider->value());
			transparency_factor_label->setText(text.c_str());
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
			setLabel_(label, ((float)slider.value()) / divisor);
		}

		void MaterialSettings::setLabel_(QLabel& label, float value)
		{
			String text(10, "%2f", value);

			while (text.has('.') && text.hasSuffix("0"))
			{
				text = text.trimRight("0");
			}

			if (text.hasSuffix(".")) text += "0";
				
			label.setText(text.c_str());
		}
		
		void MaterialSettings::setQuadraticValues_(const QSlider& slider, QLabel& label, int divisor)
		{
			float quadratic_value =((float)slider.value()) / divisor; 
			quadratic_value *= quadratic_value;
			quadratic_value *= divisor;
			QString truncated_text;
			truncated_text.setNum(quadratic_value,'f', 3);
			//String text = String(quadratic_value);
			//
			//while (text.has('.') && text.hasSuffix("0"))
			//{
			//	text = text.trimRight("0");
			//}

			//if (text.hasSuffix(".")) text += "0";
			//	
			//label.setText(text.c_str());
			label.setText(truncated_text);
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

	} // namespace VIEW
} // namespace BALL
