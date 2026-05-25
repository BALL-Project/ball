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
#include <BALL/CONCEPT/timeStamp.h>

#include <list>
#include <sstream>

namespace BALL
{
	namespace VIEW
	{

		namespace
		{
			// Compact owner-state snapshot for the ApplyPayload (§2 step 3).
			String snapshotLightState_(Stage* stage)
			{
				if (stage == nullptr) return String("light{null}");
				std::ostringstream s;
				s << "lights=" << stage->getLightSources().size();
				return String(s.str());
			}
		}

		LightController::LightController(Stage* stage, QObject* parent)
			: Controller(parent), stage_(stage),
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

		bool LightController::apply()
		{
			// 999.59-03 cut-over to the §2 `bool apply()` command contract.
			// The controller owns the COMPLETE light list (lights_); apply()
			// mirrors LightSettings::apply() (lightSettings.C:384-393): clear
			// the Stage light sources and re-add the controller's authoritative
			// list, then Scene::lightsUpdated(true) as the §2a soft refresh.

			// ── 1. Preconditions (§2 step 1) ─────────────────────────────────
			if (stage_ == nullptr)
			{
				Log.warn() << "[LightController::apply] no Stage attached — skipping." << std::endl;
				return false;                    // rejected — nothing to mutate
			}

			MainControl* mc = MainControl::getInstance(0);
			if (mc != nullptr && mc->isBusy())
			{
				Log.info() << "[LightController::apply] MainControl busy — deferring." << std::endl;
				return false;                    // rejected — busy
			}

			// ── 2. Re-entrancy shield (§2 step 2 / §4) — nest-aware drop ─────
			if (applying_depth_ > 0) return false;          // dropped — re-entry
			ControllerApplyGuard apply_guard(applying_depth_);

			// ── 3. Capture reversible intent (§2 step 3) ─────────────────────
			ApplyPayload payload;
			payload.command_id = "light.setLights";
			payload.before     = snapshotLightState_(stage_);
			payload.target     = stage_;
			payload.t_us       = PreciseTime::now().getMicroSeconds();

			// ── 4. Mutate the single owner (§2 step 4) ───────────────────────
			applyInternal_();

			payload.after = snapshotLightState_(stage_);

			// ── 5. Emit ONE typed event (§2 step 5) ──────────────────────────
			Q_EMIT appliedStub();

			// ── 6. Request the DECLARED invalidation (§2 step 6 / §2a) ───────
			invalidateDeclared_();               // soft refresh (re-light)

			// ── 7. Record reversible intent (§2 step 7) — capture only ───────
			recordIntent_(payload);

			return true;                          // mutated
		}

		bool LightController::applyInternal_()
		{
			// §13 cookbook step 2 — mutation body, moved out of apply().
			// Precondition (stage_ != nullptr) guaranteed by apply().
			//
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

			return true;
		}

		void LightController::invalidateDeclared_()
		{
			// §2a Light row — SOFT refresh. A light-list change re-lights the
			// EXISTING geometry via Scene::lightsUpdated(true); the primitive
			// set is unchanged, so no display-list rebuild is required.
			Scene* scene = Scene::getInstance(0);
			if (scene != nullptr)
			{
				scene->lightsUpdated(true);
			}
		}

		void LightController::reset()
		{
			// §2 reset path (999.64 consumes) — single-pass re-sync from owner.
			revert();
		}

		void LightController::recordIntent_(const ApplyPayload& payload)
		{
			// v1.7.4 — capture only (no UndoStack yet, §2 step 7).
			last_payload_ = payload;
		}

		void LightController::setAmbientIntensity(float v)
		{
			if (v == ambient_intensity_) return;
			ambient_intensity_ = v;
			Q_EMIT ambientIntensityChanged(v);
		}

	} // namespace VIEW
} // namespace BALL

