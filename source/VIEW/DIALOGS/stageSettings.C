// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: stageSettings.C,v 1.30.2.6 2006/04/30 13:01:49 amoll Exp $
//

#include <BALL/VIEW/DIALOGS/stageSettings.h>
#include <BALL/VIEW/WIDGETS/scene.h>
#include <BALL/VIEW/KERNEL/stage.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/clippingPlane.h>

#include <QtGui/qpushbutton.h>
#include <QtGui/qlabel.h>
#include <QtGui/qcheckbox.h>
#include <QtGui/qslider.h>
#include <QtGui/QStackedWidget>
#include <QtGui/QListWidget>

namespace BALL
{
	namespace VIEW
	{

		StageSettings::StageSettings( QWidget* parent,  const char* name, Qt::WFlags fl )
			: QWidget(parent, fl),
				Ui_StageSettingsData()
		{
			setupUi(this);
			
			// signals and slots connections
			connect( color_button, SIGNAL( clicked() ), this, SLOT( colorPressed() ) );
			connect( capping_color_button, SIGNAL( clicked() ), this, SLOT( cappingColorPressed() ) );
			connect( eye_distance_slider, SIGNAL( valueChanged(int) ), this, SLOT( eyeDistanceChanged() ) );
			connect( focal_distance_slider, SIGNAL( valueChanged(int) ), this, SLOT( focalDistanceChanged() ) );
			connect( enable_fog, SIGNAL( stateChanged(int) ), this, SLOT( fogStateChanged() ) );

			setObjectName(name);
			stage_ = ((Scene*) parent)->getStage();
			if (stage_ == 0) return;
			scene_ = (Scene*) parent;
			updateFromStage();

			registerWidgetForHelpSystem_(widget_stack->widget(2), "tips.html#3D");

			setDefaultValues_();
			setINIFileSectionName("STAGE");

			registerObject_(color_sample);
			registerObject_(use_vertex_buffers);
			registerObject_(animation_smoothness);
			registerObject_(show_lights_);
			registerObject_(enable_fog);
			registerObject_(fog_slider);
			registerObject_(popup_names);
			registerObject_(capping_color);
			registerObject_(capping_transparency);

			registerObject_(slider_);
			registerObject_(wheel_slider_);

			registerObject_(eye_distance_slider);
			registerObject_(focal_distance_slider);
			registerObject_(swap_sss_button);

			setWidgetStackName("Display");
			setWidgetStack(widget_stack);
		}


		void StageSettings::colorPressed()
		{
			chooseColor(color_sample);
		}

		void StageSettings::cappingColorPressed()
		{
			chooseColor(capping_color);
		}

		void StageSettings::updateFromStage()
			throw()
		{
			if (stage_ == 0) return;
			setColor(color_sample, stage_->getBackgroundColor());

			slider_->setValue((int) Scene::getMouseSensitivity() - 1);
			wheel_slider_->setValue((int) Scene::getMouseWheelSensitivity() - 1);

			eye_distance_slider->setValue((int) (stage_->getEyeDistance() * 10.0));
			focal_distance_slider->setValue((int) (stage_->getFocalDistance()));
			fog_slider->setValue((int) (stage_->getFogIntensity()));
			enable_fog->setChecked(stage_->getFogIntensity() > 0);
			animation_smoothness->setValue((int) (Scene::getAnimationSmoothness() * 10.0));

			ColorRGBA color = ClippingPlane::getCappingColor();
			setColor(capping_color, color);
			capping_transparency->setValue(255 - (int)color.getAlpha());

			eyeDistanceChanged();
			focalDistanceChanged();
			getGLSettings();
		}


		void StageSettings::apply()
			throw()
		{
			if (stage_ == 0) return;
			stage_->setBackgroundColor(getColor(color_sample));

			Scene::setMouseSensitivity(slider_->value() + 1);
			Scene::setMouseWheelSensitivity(wheel_slider_->value() + 1);

			if (Scene::getInstance(0) != 0)
			{
				Scene::getInstance(0)->setPopupInfosEnabled(popup_names->isChecked());
			}

			stage_->setEyeDistance((float)(eye_distance_slider->value() / 10.0));
			stage_->setFocalDistance((float)(eye_distance_slider->value()));

			stage_->setSwapSideBySideStereo(swap_sss_button->isChecked());

			if (enable_fog->isChecked())
			{
				stage_->setFogIntensity(fog_slider->value());
			}
			else
			{
				stage_->setFogIntensity(0);
			}

			Scene::setShowLightSources(show_lights_->isChecked());
			Scene::setAnimationSmoothness(((float)animation_smoothness->value()) / 10.0);

			ColorRGBA color;
			color = getColor(capping_color);
			color.setAlpha(255 - capping_transparency->value());
			ClippingPlane::getCappingColor() = color;

			// use vertex buffers ?
			bool use_buffer = use_vertex_buffers->isChecked();
			GLRenderer& renderer = ((Scene*)Scene::getInstance(0))->getGLRenderer();

			if (use_buffer != renderer.vertexBuffersEnabled())
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
		}


		void StageSettings::setDefaultValues_()
		{
			setColor(color_sample, ColorRGBA(0,0,0));
			animation_smoothness->setValue(25);
			show_lights_->setChecked(false);
			enable_fog->setChecked(false);
			fog_slider->setValue(200);
			popup_names->setChecked(false);

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

			String text(focal_distance_slider->value());
			while(text.has('.'))
			{
				text.truncate(text.size() - 1);
			}
			focal_distance_label->setText(text.c_str());
		}

		void StageSettings::fogStateChanged()
		{
			fog_slider->setEnabled(enable_fog->isChecked());
		}

		void StageSettings::getGLSettings()
			throw()
		{
			GLRenderer& renderer = ((Scene*)Scene::getInstance(0))->getGLRenderer();
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

			use_vertex_buffers->setChecked(renderer.vertexBuffersEnabled());
		}

	} // namespace VIEW
} // namespace BALL
