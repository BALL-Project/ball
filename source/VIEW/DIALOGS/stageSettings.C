// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/DIALOGS/stageSettings.h>
#include <BALL/VIEW/WIDGETS/scene.h>
#include <BALL/VIEW/KERNEL/stage.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/clippingPlane.h>
#include <BALL/VIEW/RENDERING/renderSetup.h>

#include <QtGui/QPushButton>
#include <QtGui/QLabel>
#include <QtGui/QCheckBox>
#include <QtGui/QSlider>
#include <QtGui/QStackedWidget>
#include <QtGui/QListWidget>
#include <QtGui/QFileDialog>
#include <QtGui/QImage>
#include <QtGui/QPainter>
#include <QtGui/QPainterPath>
#include <QtGui/QPixmap>
#include <QtGui/QApplication>
#include <QtGui/QDesktopWidget>
#include <QtGui/QSplashScreen>
#include <QtGui/QLinearGradient>

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

#if (QT_VERSION >= QT_VERSION_CHECK(4, 6, 0))
			screenCountChanged(QApplication::desktop()->screenCount());
#else
			screenCountChanged(QApplication::desktop()->numScreens());
#endif

			stereoModeChanged();

			controlRenderer_comboBox->addItem(tr("OpenGL"));
			controlRenderer_comboBox->setCurrentIndex(1);
			stereoScreensRenderer_comboBox->addItem(tr("OpenGL"));
			stereoScreensRenderer_comboBox->setCurrentIndex(1);

#ifndef BALL_HAS_RTFACT
			radioButton_rtfact->setEnabled(false);
			radioButton_opengl->setChecked(true);
#else
			radioButton_rtfact->setChecked(true);
			controlRenderer_comboBox->addItem(tr("RTfact"));
			stereoScreensRenderer_comboBox->addItem(tr("RTfact"));
			stereoScreensRenderer_comboBox->setCurrentIndex(2);
#endif
// TODO: this is the bad ms wischa hack!! Remove after the children are gone!
			controlRenderer_comboBox->setCurrentIndex(2);
			leftEyeScreen_comboBox->setCurrentIndex(2);
			rightEyeScreen_comboBox->setCurrentIndex(2);
			sideBySide_radioButton->setChecked(true);
			stereoModeChanged();
// END TODO
			// signals and slots connections
			connect( computeDefault_button, SIGNAL( clicked() ), this, SLOT( computeDefaultPressed() ) );
			connect( eye_distance_slider, SIGNAL( valueChanged(int) ), this, SLOT( eyeDistanceChanged() ) );
			connect( focal_distance_slider, SIGNAL( valueChanged(int) ), this, SLOT( focalDistanceChanged() ) );
			connect( radioButton_perspectiveProjection, SIGNAL( clicked() ), this, SLOT( projectionTransformationChanged() ) );

			connect( radioButton_orthographicProjection, SIGNAL( clicked() ), this, SLOT( projectionTransformationChanged() ) );
			connect( texture_browse_button, SIGNAL( clicked() ), this, SLOT( loadEnvironmentMapPressed() ) );
			connect( environment_map, SIGNAL( toggled(bool)), this, SLOT( environmentMapChanged(bool) ) );
			connect( fog_box, SIGNAL( toggled(bool)), this, SLOT( fogBoxChanged(bool) ) );	
			connect( downsampling_slider, SIGNAL( valueChanged(int) ), this, SLOT( downsamplingSliderChanged() ) );
			connect( identifyDisplays_button, SIGNAL( clicked() ), this, SLOT( identifyDisplays() ) );

			connect( QApplication::desktop(), SIGNAL( screenCountChanged(int) ), this, SLOT( screenCountChanged(int) ) );
			
			connect( interlaced_radioButton,    SIGNAL( clicked() ), this, SLOT( stereoModeChanged() ) );
			connect( sideBySide_radioButton,    SIGNAL( clicked() ), this, SLOT( stereoModeChanged() ) );
			connect( topBottom_radioButton,     SIGNAL( clicked() ), this, SLOT( stereoModeChanged() ) );
			connect( activeStereo_radioButton,  SIGNAL( clicked() ), this, SLOT( stereoModeChanged() ) );
			connect( anaglyph_radioButton,      SIGNAL( clicked() ), this, SLOT( stereoModeChanged() ) );
			connect( custom_radioButton,        SIGNAL( clicked() ), this, SLOT( stereoModeChanged() ) );

			connect( leftEyeScreen_comboBox,    SIGNAL( currentIndexChanged(int) ), this, SLOT( stereoScreenChanged(int) ) );
			connect( rightEyeScreen_comboBox,   SIGNAL( currentIndexChanged(int) ), this, SLOT( stereoScreenChanged(int) ) );
		} 


		bool StageSettings::setValueAllowed(QObject* widget)
		{
			// TODO: ms_wischa hack

			while (widget->parent())
			{
				if (widget->parent()->objectName() == "DisplaySettings") return false;
				widget = widget->parent();
			}
			return true;
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
				list<GeometricObject*>::const_iterator it = (*rit)->getGeometricObjects().begin();

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

			float focal_distance = (max_separation - min_separation)/3 + min_separation;
			float real2intern = focal_distance / getUser2ScreenDistance_();

			float eye_separation = real2intern * getUserEyeDistance_();
			
			stage_->setEyeDistance(eye_separation);
			stage_->setFocalDistance(focal_distance);
			eye_distance_slider->setValue((int) ((eye_separation) * 10.));
			focal_distance_slider->setValue((int) ((focal_distance) * 10.));
			eyeDistanceChanged();
			focalDistanceChanged();
			
			scene_->setDownsamplingFactor((float)downsamplingfactor_label->text().toFloat());
			downsamplingSliderChanged();  
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

				QImage loaded_env_map(*files.begin());
				
				scene_->setupEnvironmentMap(loaded_env_map);		
			}
		}

		void StageSettings::updateFromStage()
		{
			if (stage_ == 0) return;
			color_button->setColor(stage_->getBackgroundColor());

			slider_->setValue((int) Scene::getMouseSensitivity() - 1);
			wheel_slider_->setValue((int) Scene::getMouseWheelSensitivity() - 1);

			eye_distance_slider->setValue((int) (stage_->getEyeDistance() * 10.0));
			focal_distance_slider->setValue((int) (stage_->getFocalDistance() * 10.0));
			fog_slider->setValue((int) (stage_->getFogIntensity()));
			fog_box->setChecked(stage_->getFogIntensity() > 0);
			animation_smoothness->setValue((int) (Scene::getAnimationSmoothness() * 10.0));
			
			//TODO: integration of textures
			//environment_box->setChecked(stage->getTexture()!=""); or pointer to some texture
			//setTextureUpDirection_(stage->getTextureUpDirection());
				
			ColorRGBA color = ClippingPlane::getCappingColor();
			capping_color_button->setColor(color);
			capping_transparency->setValue(255 - (int)color.getAlpha());

			eyeDistanceChanged();
			focalDistanceChanged();
			getGLSettings();
			
			downsampling_slider->setSliderPosition(scene_->getDownsamplingFactor()*4.);
			downsamplingSliderChanged();
		}


		void StageSettings::apply()
		{
			if (stage_ == 0) return;
			stage_->setBackgroundColor(color_button->getColor());

			Scene::setMouseSensitivity(slider_->value() + 1);
			Scene::setMouseWheelSensitivity(wheel_slider_->value() + 1);

			stage_->setEyeDistance((float)(eye_distance_slider->value() / 10.0));
			stage_->setFocalDistance((float)(focal_distance_slider->value() / 10.));
			
			//TODO integration of textures
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

			Camera::ProjectionMode projection_mode = radioButton_perspectiveProjection->isChecked() ? Camera::PERSPECTIVE
			                                                                                        : Camera::ORTHOGRAPHIC;
			stage_->getCamera().setProjectionMode(projection_mode);

			Scene::setShowLightSources(show_lights_->isChecked());
			Scene::setAnimationSmoothness(((float)animation_smoothness->value()) / 10.0);
			scene_->setOffScreenRendering(offscreen_group->isChecked(), resolution_factor->value());

			ColorRGBA color;
			color = capping_color_button->getColor();
			color.setAlpha(255 - capping_transparency->value());
			ClippingPlane::getCappingColor() = color;

			scene_->setFPSEnabled(show_fps->isChecked());
			scene_->setPreview(use_preview->isChecked());

			// use vertex buffers ?
			bool use_buffer = use_vertex_buffers->isChecked() && use_vertex_buffers->isEnabled();
			GLRenderer& renderer = scene_->getGLRenderer();

			if (use_buffer != renderer.vertexBuffersEnabled() &&
					getMainControl()->getRepresentationManager().getNumberOfRepresentations() > 0)
			{
				getMainControl()->setStatusbarText(tr("Because of change in usage of vertex buffer, all Representations have to be deleted!"), true);
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

			scene_->setDownsamplingFactor(downsamplingfactor_label->text().toFloat());

			if (radioButton_opengl->isChecked())
				scene_->switchRenderer(RenderSetup::OPENGL_RENDERER);
			else
				scene_->switchRenderer(RenderSetup::RTFACT_RENDERER);
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
			color_button->setColor(QColor(0,0,0));
			animation_smoothness->setValue(25);
			show_lights_->setChecked(false);
			smooth_lines_->setChecked(false);
			fog_box->setChecked(false);
			fog_slider->setValue(200);
//			environment_map->setDisabled(true);
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
			downsampling_slider->setValue(4);
		}

		void StageSettings::stereoScreenChanged(int new_screen)
		{
			stereoModeChanged();
		}

		void StageSettings::identifyDisplays()
		{
			// loop over all displays
			// (we iterate until size() - 1 because there's also
			// 'disabled' in the combo box)
			for (int i=0; i<std::max(0, controlScreen_comboBox->count() - 1); ++i)
			{
				QPainter p;

				// create a transparent background image
				QImage pm(250, 250, QImage::Format_ARGB32);
				pm.fill(qRgba(0, 0, 0, 0));

				p.begin(&pm);

				QFont text_font("Arial", 200, QFont::Bold);

				QPainterPath text_path;
				text_path.addText(QPointF(10.f, 220.f), text_font, QString::number(i));

				QLinearGradient gradient(0, 0, 0, 100);
				gradient.setColorAt(0.0, qRgba(0, 0, 128, 0));
				gradient.setColorAt(1.0, qRgba(0, 128, 128, 0));

				p.setBrush(gradient);
				p.setPen(QPen(QColor(0, 0, 0), 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

				p.drawPath(text_path);
				p.end();

				pm.createAlphaMask();

				QSplashScreen* splash = new QSplashScreen(QApplication::desktop()->screen(i), QPixmap::fromImage(pm));
				splash->show();

				identification_labels_.push_back(splash);
			}

			QTimer::singleShot(2000, this, SLOT(killIdentificationLabels_()));
		}

		void StageSettings::stereoModeChanged()
		{
			int control_screen_index = getControlScreenNumber();
			int left_screen_index    = getLeftEyeScreenNumber();
			int right_screen_index   = getRightEyeScreenNumber();

			if (control_screen_index == -1 || left_screen_index == -1 || right_screen_index == -1)
				return;

			QDesktopWidget* desktop = QApplication::desktop();

			QRect left_screen_geom  = QApplication::desktop()->screenGeometry(left_screen_index);
			QRect	right_screen_geom = QApplication::desktop()->screenGeometry(right_screen_index);

			if (left_screen_index == right_screen_index)
			{
				if (sideBySide_radioButton->isChecked())
				{
					left_screen_geom.setWidth(left_screen_geom.width()/2);

					right_screen_geom.setX(left_screen_geom.x() + left_screen_geom.width());
					right_screen_geom.setWidth(left_screen_geom.width());
				}
				else if (topBottom_radioButton->isChecked())
				{
					left_screen_geom.setHeight(left_screen_geom.height()/2);

					right_screen_geom.setY(left_screen_geom.y() + left_screen_geom.height());
					right_screen_geom.setHeight(left_screen_geom.height());
				}
			}

			if (sideBySide_radioButton->isChecked() || topBottom_radioButton->isChecked())
			{
				left_xmin_edit->setText(QString::number(left_screen_geom.x()));
				left_ymin_edit->setText(QString::number(left_screen_geom.y()));
				left_width_edit->setText(QString::number(left_screen_geom.width()));
				left_height_edit->setText(QString::number(left_screen_geom.height()));

				right_xmin_edit->setText(QString::number(right_screen_geom.x()));
				right_ymin_edit->setText(QString::number(right_screen_geom.y()));
				right_width_edit->setText(QString::number(right_screen_geom.width()));
				right_height_edit->setText(QString::number(right_screen_geom.height()));
			}
		}

		void StageSettings::killIdentificationLabels_()
		{
			for (Position i=0; i<identification_labels_.size(); ++i)
			{
				delete(identification_labels_[i]);
			}

			identification_labels_.clear();
		}

		int StageSettings::getControlScreenNumber() const
		{
			bool valid;
			int result = controlScreen_comboBox->currentText().toInt(&valid);

			if (!valid)
				result = -1;

			return result;
		}

		int StageSettings::getLeftEyeScreenNumber() const
		{
			bool valid;
			int result = leftEyeScreen_comboBox->currentText().toInt(&valid);

			if (!valid)
				result = -1;

			return result;
		}

		int StageSettings::getRightEyeScreenNumber() const
		{
			bool valid;
			int result = rightEyeScreen_comboBox->currentText().toInt(&valid);

			if (!valid)
				result = -1;

			return result;
		}

		Renderer::StereoMode StageSettings::getStereoMode() const
		{
			Renderer::StereoMode result = Renderer::NO_STEREO;

			if (interlaced_radioButton->isChecked())
				result = Renderer::INTERLACED_STEREO;
			else if (sideBySide_radioButton->isChecked())
				result = Renderer::DUAL_VIEW_STEREO;
			else if (topBottom_radioButton->isChecked())
				result = Renderer::TOP_BOTTOM_STEREO;
			else if (activeStereo_radioButton->isChecked())
				result = Renderer::ACTIVE_STEREO;

			return result;
		}

		RenderSetup::RendererType StageSettings::getControlScreenRendererType() const
		{
			RenderSetup::RendererType result = RenderSetup::OPENGL_RENDERER;

			QString selected_renderer = controlRenderer_comboBox->currentText();

			if (selected_renderer == "OpenGL")
				result = RenderSetup::OPENGL_RENDERER;
			else if (selected_renderer == "RTfact")
				result = RenderSetup::RTFACT_RENDERER;

			return result;
		}

		RenderSetup::RendererType StageSettings::getStereoScreensRendererType() const
		{
			RenderSetup::RendererType result = RenderSetup::OPENGL_RENDERER;

			QString selected_renderer = stereoScreensRenderer_comboBox->currentText();

			if (selected_renderer == "OpenGL")
				result = RenderSetup::OPENGL_RENDERER;
			else if (selected_renderer == "RTfact")
				result = RenderSetup::RTFACT_RENDERER;

			return result;
		}

		QRect StageSettings::getLeftEyeGeometry() const
		{
			bool valid;
			int left_xmin_result = left_xmin_edit->text().toInt(&valid);
			if (!valid) left_xmin_result = 0;

			int left_ymin_result = left_ymin_edit->text().toInt(&valid);
			if (!valid) left_ymin_result = 0;

			int left_width_result = left_width_edit->text().toInt(&valid);
			if (!valid) left_width_result = 0;

			int left_height_result = left_height_edit->text().toInt(&valid);
			if (!valid) left_height_result = 0;

			QRect result(left_xmin_result, left_ymin_result, left_width_result, left_height_result);

			return result;
		}

		QRect StageSettings::getRightEyeGeometry() const
		{
			bool valid;
			int right_xmin_result = right_xmin_edit->text().toInt(&valid);
			if (!valid) right_xmin_result = 0;

			int right_ymin_result = right_ymin_edit->text().toInt(&valid);
			if (!valid) right_ymin_result = 0;

			int right_width_result = right_width_edit->text().toInt(&valid);
			if (!valid) right_width_result = 0;

			int right_height_result = right_height_edit->text().toInt(&valid);
			if (!valid) right_height_result = 0;

			QRect result(right_xmin_result, right_ymin_result, right_width_result, right_height_result);

			return result;
		}

		void StageSettings::screenCountChanged(int number)
		{
			// first, get the old mappings
			int old_control = controlScreen_comboBox->currentIndex();
			int old_left    = leftEyeScreen_comboBox->currentIndex();
			int old_right   = rightEyeScreen_comboBox->currentIndex();

			// TODO: we need a heuristic to map the old values to sensible new ones...

			controlScreen_comboBox->clear();
			controlScreen_comboBox->addItem(tr("disabled"));
			for (int i=0; i<number; ++i)
				controlScreen_comboBox->addItem(QString::number(i));
			controlScreen_comboBox->setCurrentIndex(1);

			leftEyeScreen_comboBox->clear();
			leftEyeScreen_comboBox->addItem(tr("disabled"));
			for (int i=0; i<number; ++i)
				leftEyeScreen_comboBox->addItem(QString::number(i));

			rightEyeScreen_comboBox->clear();
			rightEyeScreen_comboBox->addItem(tr("disabled"));
			for (int i=0; i<number; ++i)
				rightEyeScreen_comboBox->addItem(QString::number(i));
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
			//TODO ersetzen durch eye_distance_label->setText(createFloatString(eye_distance_slider->value()) / 10.0));
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
			//TODO ersetzen durch focal_distance_label->setText(createFloatString(focal_distance_slider->value()) / 10.0));
		}
	
		void StageSettings::projectionTransformationChanged()
		{
			Camera::ProjectionMode projection_mode;
			if (radioButton_perspectiveProjection->isChecked())
			{
				projection_mode = Camera::PERSPECTIVE;
			}
			else
			{
				projection_mode = Camera::ORTHOGRAPHIC;
			}	

			stage_->getCamera().setProjectionMode(projection_mode);
			scene_->projectionModeChanged();
		}

		void StageSettings::downsamplingSliderChanged()
		{
			if (downsampling_slider->value() == 0)
			{
				downsamplingfactor_label->setText("0");
				return;
			}
			downsamplingfactor_label->setText(createFloatString( (downsampling_slider->value()/4.0), 2).c_str()); 
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
