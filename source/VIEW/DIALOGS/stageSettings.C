// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: stageSettings.C,v 1.26 2005/03/02 09:46:06 oliver Exp $
//

#include <BALL/VIEW/DIALOGS/stageSettings.h>
#include <BALL/VIEW/WIDGETS/scene.h>
#include <BALL/VIEW/KERNEL/stage.h>
#include <BALL/VIEW/KERNEL/mainControl.h>

#include <qpushbutton.h>
#include <qlabel.h>
#include <qcheckbox.h>
#include <qslider.h>
#include <qwidgetstack.h>
#include <qlistview.h>

namespace BALL
{
	namespace VIEW
	{

		StageSettings::StageSettings( QWidget* parent,  const char* name, WFlags fl )
			: StageSettingsData(parent, name, fl),
				PreferencesEntry()
		{
			stage_ = ((Scene*) parent)->getStage();
			if (stage_ == 0) return;
			scene_ = (Scene*) parent;
			updateFromStage();

			insertEntry(this, "Display");
			setWidgetStack(widget_stack);
		}


		void StageSettings::colorPressed()
		{
			chooseColor(color_sample);
		}


		void StageSettings::updateFromStage()
			throw()
		{
			if (stage_ == 0) return;
			color_sample->setBackgroundColor(stage_->getBackgroundColor().getQColor());
			coordinate_button->setChecked(stage_->coordinateSystemEnabled());

			slider_->setValue((int) Scene::getMouseSensitivity() - 1);
			wheel_slider_->setValue((int) Scene::getMouseWheelSensitivity() - 1);

			eye_distance_slider->setValue((int) (stage_->getEyeDistance() * 10.0));
			focal_distance_slider->setValue((int) (stage_->getFocalDistance()));
			fog_slider->setValue((int) (stage_->getFogIntensity()));
			enable_fog->setChecked(stage_->getFogIntensity() > 0);
			animation_smoothness->setValue((int) (Scene::getAnimationSmoothness() * 10.0));
			eyeDistanceChanged();
			focalDistanceChanged();
			getGLSettings();
		}


		void StageSettings::apply()
			throw()
		{
			if (stage_ == 0) return;
			stage_->setBackgroundColor(color_sample->backgroundColor());
			stage_->showCoordinateSystem(coordinate_button->isChecked());

			Scene::setMouseSensitivity(slider_->value() + 1);
			Scene::setMouseWheelSensitivity(wheel_slider_->value() + 1);

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

			/* ?????
			// use vertex buffers ?
			bool use_buffer = use_vertex_buffers->isChecked();
			GLRenderer& renderer = ((Scene*)Scene::getInstance(0))->getGLRenderer();

			if (use_buffer != renderer.vertexBuffersEnabled())
			{
				getMainControl()->setStatusbarText("Because of change in usage of vertex buffer, all Representations have to be deleted!", true);
				// remove representations
				PrimitiveManager& pm = getMainControl()->getPrimitiveManager();
				Size nr = pm.getNumberOfRepresentations();
				list<Representation*> reps = pm.getRepresentations();
				for (Position p = 0; p < nr; p++)
				{
					getMainControl()->remove(**reps.begin());
					reps.pop_front();
				}
			}

			renderer.enableVertexBuffers(use_buffer);
			*/ // ?????
		}


		void StageSettings::setDefaultValues(bool all)
			throw()
		{
			Position current = widget_stack->id(widget_stack->visibleWidget());
			if (all || current == 0)
			{
				color_sample->setBackgroundColor(black);
				animation_smoothness->setValue(25);
				coordinate_button->setChecked(false);
				show_lights_->setChecked(false);

				enable_fog->setChecked(false);
				fog_slider->setValue(200);
			}

			if (all || current == 1)
			{
				slider_->setValue(5);
				wheel_slider_->setValue(5);
			}

			if (all || current == 2)
			{
				eye_distance_slider->setValue(20);
				focal_distance_slider->setValue(40);
				swap_sss_button->setChecked(false);
			}

			/**
			if (use_vertex_buffers->isEnabled())
			{
				use_vertex_buffers->setChecked(true);
			}
			*/ // ?????
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
				QListViewItem* item = new QListViewItem(extensions_list, extensions[p].c_str());
				extensions_list->insertItem(item);
			}

			if (!renderer.vertexBuffersSupported())
			{
				use_vertex_buffers->setEnabled(false);
			}

			use_vertex_buffers->setChecked(renderer.vertexBuffersEnabled());
		}

	} // namespace VIEW

} // namespace BALL
