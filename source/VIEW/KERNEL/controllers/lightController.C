// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 Plan 05 — LightController implementation.
// UFG-11 cut-over (v1.7-modernization) — apply() now mutates the
// attached Stage's LightSource list (ambient component) and triggers
// a Scene::lightsUpdated() refresh, mirroring the legacy
// LightSettings::apply() backend (source/VIEW/DIALOGS/lightSettings.C
// :384-393). Inspector controller exposes a single "Ambient" slider;
// non-ambient lights are preserved as-is. Per-light editing remains
// in the legacy LightSettings dialog reachable via Tools › Legacy
// Settings during the migration window.
//

#include <BALL/VIEW/KERNEL/controllers/lightController.h>


#include <BALL/VIEW/KERNEL/stage.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/WIDGETS/scene.h>
#include <BALL/VIEW/DATATYPE/colorRGBA.h>
#include <BALL/COMMON/logStream.h>

#include <list>

namespace BALL
{
	namespace VIEW
	{

		LightController::LightController(Stage* stage, QObject* parent)
			: QObject(parent), stage_(stage),
				light_count_(0), ambient_intensity_(0.3f)
		{
			revert();
		}

		LightController::~LightController() = default;

		void LightController::setStage(Stage* stage)
		{
			if (stage_ == stage) return;
			stage_ = stage;
			revert();
		}

		void LightController::revert()
		{
			if (stage_ == nullptr) return;
			const std::list<LightSource>& lights = stage_->getLightSources();
			int n = static_cast<int>(lights.size());
			if (n != light_count_)
			{
				light_count_ = n;
				Q_EMIT lightCountChanged(n);
			}
			// Pull ambient from the first AMBIENT-type LightSource so
			// the Inspector mirrors live Scene state (UFG-12 fix — no
			// more blank-on-tab-switch). If none exists, leave ambient
			// at its current value (controller default 0.3 on construct).
			for (std::list<LightSource>::const_iterator it = lights.begin();
			     it != lights.end(); ++it)
			{
				if (it->getType() == LightSource::AMBIENT)
				{
					float a = it->getIntensity();
					if (a != ambient_intensity_)
					{
						ambient_intensity_ = a;
						Q_EMIT ambientIntensityChanged(a);
					}
					break;
				}
			}
		}

		void LightController::apply()
		{
			// UFG-11 cut-over — push ambient_intensity_ to the Stage by
			// upserting a single AMBIENT-type LightSource. Non-ambient
			// lights (POSITIONAL / DIRECTIONAL) are preserved verbatim;
			// per-light editing stays in the legacy LightSettings dialog
			// during the migration window. After mutation we call
			// Scene::lightsUpdated(true) — same redraw path the legacy
			// dialog uses (LightSettings.C:150, 162, 263, 279, 403).
			if (stage_ == nullptr)
			{
				Log.warn() << "[LightController::apply] no Stage attached — skipping." << std::endl;
				return;
			}

			MainControl* mc = MainControl::getInstance(0);
			if (mc != nullptr && mc->isBusy())
			{
				Log.info() << "[LightController::apply] MainControl busy — deferring." << std::endl;
				return;
			}

			// Snapshot non-ambient lights, clear, re-add them, then
			// append one ambient at ambient_intensity_. This is the
			// same idiom as LightSettings::apply() — clear + re-add
			// the controller's authoritative list.
			std::list<LightSource> preserved;
			for (std::list<LightSource>::const_iterator it = stage_->getLightSources().begin();
			     it != stage_->getLightSources().end(); ++it)
			{
				if (it->getType() != LightSource::AMBIENT)
					preserved.push_back(*it);
			}

			stage_->clearLightSources();
			for (std::list<LightSource>::const_iterator it = preserved.begin();
			     it != preserved.end(); ++it)
			{
				stage_->addLightSource(*it);
			}

			LightSource amb;
			amb.setType(LightSource::AMBIENT);
			amb.setIntensity(ambient_intensity_);
			amb.setColor(ColorRGBA(255, 255, 255, 255));
			amb.setRelativeToCamera(false);
			stage_->addLightSource(amb);

			// Update controller-side count so subsequent revert() / UI
			// reads see the new count.
			int new_count = static_cast<int>(stage_->getLightSources().size());
			if (new_count != light_count_)
			{
				light_count_ = new_count;
				Q_EMIT lightCountChanged(new_count);
			}

			// Trigger GL re-light + redraw via Scene.
			Scene* scene = Scene::getInstance(0);
			if (scene != nullptr)
			{
				scene->lightsUpdated(true);
			}

			Q_EMIT appliedStub();
		}

		void LightController::setAmbientIntensity(float v)
		{
			if (v == ambient_intensity_) return;
			ambient_intensity_ = v;
			Q_EMIT ambientIntensityChanged(v);
		}

	} // namespace VIEW
} // namespace BALL

