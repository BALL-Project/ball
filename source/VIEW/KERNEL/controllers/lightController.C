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
#include <BALL/VIEW/KERNEL/controllers/controllerApplyGuard.h>
#include <BALL/COMMON/logStream.h>

#include <list>

namespace BALL
{
	namespace VIEW
	{

		LightController::LightController(Stage* stage, QObject* parent)
			: QObject(parent), stage_(stage),
				light_count_(0), ambient_intensity_(0.3f),
				applying_(false)
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

			// 999.58-01 — pull the FULL live light list into the
			// controller's authoritative lights_ vector (mirrors
			// LightSettings::updateFromStage, lightSettings.C:69-86).
			// The controller now owns every light, not just ambient.
			const std::list<LightSource>& lights = stage_->getLightSources();
			lights_.clear();
			for (std::list<LightSource>::const_iterator it = lights.begin();
			     it != lights.end(); ++it)
			{
				lights_.push_back(*it);
			}

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

		void LightController::syncFromStage()
		{
			// Public stage-resync entry point for the Scene render-refresh /
			// camera-move call sites (replaces light_settings_->updateFromStage()
			// in scene.C). Re-reading the full Stage light list IS what revert()
			// does, so this is a thin, intention-revealing alias.
			revert();
		}

		void LightController::setLightSources(const std::vector<LightSource>& lights)
		{
			lights_ = lights;
			int n = static_cast<int>(lights_.size());
			if (n != light_count_)
			{
				light_count_ = n;
				Q_EMIT lightCountChanged(n);
			}
		}

		void LightController::apply()
		{
			// 999.58-01 cut-over — the controller now owns the COMPLETE
			// light list (lights_). apply() mirrors LightSettings::apply()
			// (lightSettings.C:384-393): clear the Stage light sources and
			// re-add the controller's authoritative list, then
			// Scene::lightsUpdated(true) — same redraw path the legacy dialog
			// uses (LightSettings.C:150, 162, 263, 279, 403).
			//
			// The aggregate ambient-intensity slider (Inspector LightsSection)
			// stays meaningful: the AMBIENT light in lights_ is kept in sync
			// with ambient_intensity_, and an AMBIENT default is injected for
			// an empty list so existing rendered scenes are unchanged
			// (mirrors defaultsPressed()).
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

			// v1.7.x-24 — re-entrancy shield. If a notification triggered by
			// this apply() (e.g. the lightsUpdated() refresh below)
			// synchronously re-enters apply(), bail rather than re-running the
			// mutation — this is the cascade class behind the v1.7.x-13 freeze.
			// The RAII guard clears the flag on every exit path.
			if (applying_) return;
			ControllerApplyGuard apply_guard(applying_);

			// Re-add every non-ambient light the controller owns, then append
			// exactly one AMBIENT light at the aggregate ambient_intensity_.
			// If lights_ already carries an AMBIENT entry, its intensity is
			// driven by the slider value (ambient_intensity_) so the two stay
			// consistent; non-ambient AMBIENT duplicates are collapsed.
			stage_->clearLightSources();

			bool wrote_any_non_ambient = false;
			for (std::vector<LightSource>::const_iterator it = lights_.begin();
			     it != lights_.end(); ++it)
			{
				if (it->getType() != LightSource::AMBIENT)
				{
					stage_->addLightSource(*it);
					wrote_any_non_ambient = true;
				}
			}

			// Locate the controller-owned AMBIENT entry (if any) so we
			// preserve its color while honoring the slider intensity.
			ColorRGBA ambient_color(255, 255, 255, 255);
			for (std::vector<LightSource>::const_iterator it = lights_.begin();
			     it != lights_.end(); ++it)
			{
				if (it->getType() == LightSource::AMBIENT)
				{
					ambient_color = it->getColor();
					break;
				}
			}

			LightSource amb;
			amb.setType(LightSource::AMBIENT);
			amb.setIntensity(ambient_intensity_);
			amb.setColor(ambient_color);
			amb.setRelativeToCamera(false);
			stage_->addLightSource(amb);

			(void)wrote_any_non_ambient;

			// Keep the controller's authoritative list in sync with what we
			// just pushed to the Stage so subsequent revert()/reads agree.
			const std::list<LightSource>& applied = stage_->getLightSources();
			lights_.assign(applied.begin(), applied.end());

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

