// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: stageSettings.C,v 1.30.16.1 2007/03/25 22:02:19 oliver Exp $
//

#include <BALL/VIEW/DIALOGS/stageSettings.h>
#include <BALL/VIEW/WIDGETS/scene.h>
#include <BALL/VIEW/KERNEL/stage.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/clippingPlane.h>

#include <QtGui/QPushButton>
#include <QtGui/QLabel>
#include <QtGui/QCheckBox>
#include <QtGui/QSlider>
#include <QtGui/QStackedWidget>
#include <QtGui/QListWidget>
#include <QtGui/QFileDialog>

namespace BALL
{
	namespace VIEW
	{

		StageSettings::StageSettings( QWidget* parent,  const char* name, Qt::WFlags fl )
			: QWidget(parent, fl),
				Ui_StageSettingsData(),
				PreferencesEntry()
		{
			setupUi(this);

			setObjectName(name);
			stage_ = ((Scene*) parent)->getStage();
			if (stage_ == 0) return;
			scene_ = (Scene*) parent;
			updateFromStage();

			registerWidgetForHelpSystem_(widget_stack->widget(2), "tips.html#3D");

			setDefaultValues_();
			setINIFileSectionName("STAGE");
			setWidgetStackName("Display");
			widget_stack->removeWidget(RTFact);
			setWidgetStack(widget_stack);
			registerWidgets_();

			// signals and slots connections
			connect( color_button, SIGNAL( clicked() ), this, SLOT( colorPressed() ) );
			connect( computeDefault_button, SIGNAL( clicked() ), this, SLOT( computeDefaultPressed() ) );
			connect( capping_color_button, SIGNAL( clicked() ), this, SLOT( cappingColorPressed() ) );
			connect( eye_distance_slider, SIGNAL( valueChanged(int) ), this, SLOT( eyeDistanceChanged() ) );
			connect( focal_distance_slider, SIGNAL( valueChanged(int) ), this, SLOT( focalDistanceChanged() ) );
			connect( radioButton_perspectiveProjection, SIGNAL( clicked() ), this, SLOT( projectionTransformationChanged()));
			connect( radioButton_orthographicProjection, SIGNAL( clicked() ), this, SLOT( projectionTransformationChanged()));
			connect( texture_browse_button, SIGNAL( clicked() ), this, SLOT( loadEnvironmentMapPressed()));
			connect( environment_map, SIGNAL( toggled(bool)), this, SLOT(environmentMapChanged(bool)));
			connect( fog_box, SIGNAL( toggled(bool)), this, SLOT(fogBoxChanged(bool)));
		} 


		void StageSettings::colorPressed()
		{
			chooseColor(color_sample);
		}
	
		void StageSettings::computeDefaultPressed()
		{
			// try to compute sensible stereo settings
			Camera& camera = scene_->getStage()->getCamera();
			Vector3 view_vector = camera.getViewVector();
			view_vector.normalize();
			const Vector3& view_point = camera.getViewPoint();

			RepresentationManager& r = MainControl::getInstance(0)->getRepresentationManager();
			RepresentationList::const_iterator rit = r.getRepresentations().begin();

			float min_separation = 600;
			float max_separation = 1.5;

			for (; rit != r.getRepresentations().end(); ++rit)
			{
				List<GeometricObject*>::ConstIterator it = (*rit)->getGeometricObjects().begin();

				vector<Vector3> positions;
				for (; it != (*rit)->getGeometricObjects().end(); ++it)
				{
					(*it)->getVertices(positions);
					// iterate over them and find the closest point along the view vector
					for (Position i=0; i<positions.size(); i++)
					{
						float separation = (positions[i] - view_point)*view_vector;
						min_separation = std::min(std::max(separation, 0.f), min_separation);
						max_separation = std::max(std::max(separation, 0.f), max_separation);
					}
					positions.clear();
				}
			}
			Log.info() << "min_sep " << min_separation << std::endl;
			Log.info() << "max_sep " << max_separation << std::endl;

			float focal_distance = (max_separation - min_separation)/3 + min_separation;
			Log.info() << "focal distance  " << focal_distance << std::endl;

			float real2intern = focal_distance / getUser2ScreenDistance_();
			Log.info() << "screen2intern  " << real2intern << std::endl;

			float eye_separation = real2intern * getUserEyeDistance_();
			
			stage_->setEyeDistance(eye_separation);
			stage_->setFocalDistance(focal_distance);
			eye_distance_slider->setValue((int) ((eye_separation) * 10.));
			focal_distance_slider->setValue((int) ((focal_distance) * 10.));
			eyeDistanceChanged();
			focalDistanceChanged();
		}
		
		void StageSettings::environmentMapChanged(bool active)
		{
 			if (active)
			{
				setTextureUpDirection_(stage_->getCamera().getLookUpVector());	
			}
		}
		
		void StageSettings::fogBoxChanged(bool active)
		{
			fog_slider->setEnabled(active);
		}


		void StageSettings::loadEnvironmentMapPressed()
		{
			//TODO set the file filter for texture files correctly!
			QStringList files = QFileDialog::getOpenFileNames(
					0,
					"Choose an environment texture file to open",
					scene_->getWorkingDir().c_str(),
					"*");
			if (files.begin() != files.end())
			{
				texture_file_edit->setText(*files.begin());
				//TODO open this file and prepare for stage or submit the filename to the stage or scene
			}
		}

		void StageSettings::cappingColorPressed()
		{
			chooseColor(capping_color);
		}

		void StageSettings::updateFromStage()
		{
			if (stage_ == 0) return;
			setColor(color_sample, stage_->getBackgroundColor());

			slider_->setValue((int) Scene::getMouseSensitivity() - 1);
			wheel_slider_->setValue((int) Scene::getMouseWheelSensitivity() - 1);

			eye_distance_slider->setValue((int) (stage_->getEyeDistance() * 10.0));
			focal_distance_slider->setValue((int) (stage_->getFocalDistance() * 10.0));
			fog_slider->setValue((int) (stage_->getFogIntensity()));
			fog_box->setChecked(stage_->getFogIntensity() > 0);
			animation_smoothness->setValue((int) (Scene::getAnimationSmoothness() * 10.0));
			
			//TODO
			//environment_box->setChecked(stage->getTexture()!=""); or pointer to some texture
			//setTextureUpDirection_(stage->getTextureUpDirection());
				
			ColorRGBA color = ClippingPlane::getCappingColor();
			setColor(capping_color, color);
			capping_transparency->setValue(255 - (int)color.getAlpha());

			eyeDistanceChanged();
			focalDistanceChanged();

			getGLSettings();
		}


		void StageSettings::apply()
		{
			if (stage_ == 0) return;
			stage_->setBackgroundColor(getColor(color_sample));

			Scene::setMouseSensitivity(slider_->value() + 1);
			Scene::setMouseWheelSensitivity(wheel_slider_->value() + 1);

			stage_->setEyeDistance((float)(eye_distance_slider->value() / 10.0));
			stage_->setFocalDistance((float)(focal_distance_slider->value() / 10.));
			
			//TODO
			//if (environement_map->isChecked())
			//{
			//   stage->setTextureUpDirection(getTextureUpDirection_()));	
			//   String filenname = ascii(texture_file_edit->text()));
			//   if (filename is valid?)
			//     stage->setTexture(filename));
			//}

			stage_->setSwapSideBySideStereo(swap_sss_button->isChecked());

			if (fog_box->isChecked())
			{
				stage_->setFogIntensity(fog_slider->value());
			}
			else
			{
				stage_->setFogIntensity(0);
			}

			//TODO see projectionTransformationChanged() for code examples.
			//stage_->setPerspectiveProjection(radioButton_perspectiveProjection->isChecked());

			Scene::setShowLightSources(show_lights_->isChecked());
			Scene::setAnimationSmoothness(((float)animation_smoothness->value()) / 10.0);
			scene_->setOffScreenRendering(offscreen_group->isChecked(), resolution_factor->value());

			ColorRGBA color;
			color = getColor(capping_color);
			color.setAlpha(255 - capping_transparency->value());
			ClippingPlane::getCappingColor() = color;

			scene_->setFPSEnabled(show_fps->isChecked());
			scene_->setPreview(use_preview->isChecked());

			// use vertex buffers ?
			bool use_buffer = use_vertex_buffers->isChecked();
			GLRenderer& renderer = scene_->getGLRenderer();

			if (use_buffer != renderer.vertexBuffersEnabled() &&
					getMainControl()->getRepresentationManager().getNumberOfRepresentations() > 0)
			{
				getMainControl()->setStatusbarText("Because of change in usage of vertex buffer, all Representations have to be deleted!", true);
				// remove representations
				RepresentationManager& pm = getMainControl()->getRepresentationManager();
				Size nr = pm.getNumberOfRepresentations();
				list<Representation*> reps = pm.getRepresentations();
				for (Position p = 0; p < nr; p++)
				{
					getMainControl()->remove(**reps.begin());
					reps.pop_front();
				}
			}

			renderer.enableVertexBuffers(use_buffer);
			renderer.setSmoothLines(smooth_lines_->isChecked());
		}

		Vector3 StageSettings::getTextureUpDirection_()
			throw(Exception::InvalidFormat)
		{
			return Vector3(up_direction_x->text().toFloat(),
				 			   up_direction_y->text().toFloat(),
			  				 up_direction_z->text().toFloat());
		}

		void StageSettings::setTextureUpDirection_(const Vector3& tud)
		{
			up_direction_x->setText(createFloatString(tud.x, 2).c_str());
			up_direction_y->setText(createFloatString(tud.y, 2).c_str());
			up_direction_z->setText(createFloatString(tud.z, 2).c_str());
		}


		float StageSettings::getUser2ScreenDistance_()
			throw(Exception::InvalidFormat)
		{
			return screen_distance_lineEdit->text().toFloat();
		}

		void StageSettings::setUser2ScreenDistance_(const float& s2u)
		{
			screen_distance_lineEdit->setText(createFloatString(s2u, 2).c_str());
		}

		float StageSettings::getUserEyeDistance_()
			throw(Exception::InvalidFormat)
		{
			return eye_distance_lineEdit->text().toFloat();
		}

		void StageSettings::setUserEyeDistance_(const float& s2u)
		{
			eye_distance_lineEdit->setText(createFloatString(s2u, 2).c_str());
		}
		
		float StageSettings::getUserEyeLevel_()
			throw(Exception::InvalidFormat)
		{
			return eye_level_lineEdit->text().toFloat();
		}

		void StageSettings::setUserEyeLevel_(const float& s2u)
		{
			eye_level_lineEdit->setText(createFloatString(s2u, 2).c_str());
		}
		
		void StageSettings::setDefaultValues_()
		{
			setColor(color_sample, ColorRGBA(0,0,0));
			animation_smoothness->setValue(25);
			show_lights_->setChecked(false);
			smooth_lines_->setChecked(false);
			fog_box->setChecked(false);
			fog_slider->setValue(200);
			environment_map->setDisabled(true);
			environment_map->setChecked(false);
			
			setTextureUpDirection_(stage_->getCamera().getLookUpVector());	
			texture_file_edit->setText("texture");
			radioButton_perspectiveProjection->setChecked(true);
			radioButton_orthographicProjection->setChecked(false);
			slider_->setValue(5);
			wheel_slider_->setValue(5);

			eye_distance_slider->setValue(20);
			focal_distance_slider->setValue(40);
			swap_sss_button->setChecked(false);

			if (use_vertex_buffers->isEnabled())
			{
				use_vertex_buffers->setChecked(true);
			}
		}

		void StageSettings::eyeDistanceChanged()
		{
			if (eye_distance_slider->value() == 0)
			{
				eye_distance_label->setText("0");
				return;
			}

			String text((float)(eye_distance_slider->value()) / 10.0);
			while(text[text.size() -1] == '.' || text[text.size() -1] == '0')
			{
				text.truncate(text.size() - 1);
			}
			eye_distance_label->setText(text.c_str());
		}

		void StageSettings::focalDistanceChanged()
		{
			if (focal_distance_slider->value() == 0)
			{
				focal_distance_label->setText("0");
				return;
			}

			String text(focal_distance_slider->value() / 10.0);
			while(text.has('.'))
			{
				text.truncate(text.size() - 1);
			}
			focal_distance_label->setText(text.c_str());
		}
	
		void StageSettings::projectionTransformationChanged()
		{
			if (radioButton_perspectiveProjection->isChecked())
			{
				Log.info() << "Switched to perspective projection." << std::endl;
					// glMatrixMode(GL_PROJECTION);
					// glLoadIdentity();
					// glFrustrum(left, right, bottom, top, near, far);	
					//stage_->setPerspectiveProjection(radioButton_perspectiveProjection->isChecked());
			}
			else if (radioButton_orthographicProjection->isChecked())
			{
				Log.info() << "Switched to orthographic projection." << std::endl;
				// glMatrixMode(GL_PROJECTION);
				// glLoadIdentity();
				// glOrtho(left, right, bottom, top, near, far);
			}	
		}

		// TODO: rewrite to allow more than one renderer
		void StageSettings::getGLSettings()
		{
			GLRenderer& renderer = scene_->getGLRenderer();
			if (renderer.getVendor() == "") return;
			vendor_label->setText(renderer.getVendor().c_str());
			version_label->setText(renderer.getOpenGLVersion().c_str());
			renderer_label->setText(renderer.getRenderer().c_str());
			extensions_list->clear();
			vector<String> extensions = renderer.getExtensions();

			for (Position p = 0; p < extensions.size(); p++)
			{
				new QListWidgetItem(extensions[p].c_str(), extensions_list);
			}

			if (!renderer.vertexBuffersSupported())
			{
				use_vertex_buffers->setEnabled(false);
			}

			smooth_lines_->setChecked(renderer.getSmoothLines());
			use_vertex_buffers->setChecked(renderer.vertexBuffersEnabled());
			
			//TODO 
			/*bool perspectiveProjection = renderer_->isPerspectiveProjection();
			radioButton_perspectiveProjection->setChecked(perspectiveProjection);
			radioButton_orthographicProjection->setChecked(!perspectiveProjection);*/

		}

	} // namespace VIEW
} // namespace BALL
