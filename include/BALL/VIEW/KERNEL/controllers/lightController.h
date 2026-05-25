// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 Plan 05 — LightController (read-only mirror).
//
// Source dialog: source/VIEW/DIALOGS/lightSettings.C (~538 LOC).
//

#ifndef BALL_VIEW_KERNEL_CONTROLLERS_LIGHTCONTROLLER_H
#define BALL_VIEW_KERNEL_CONTROLLERS_LIGHTCONTROLLER_H


#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#ifndef BALL_VIEW_KERNEL_CONTROLLERS_CONTROLLER_H
# include <BALL/VIEW/KERNEL/controllers/controller.h>
#endif

#ifndef BALL_VIEW_KERNEL_CONTROLLERS_APPLYPAYLOAD_H
# include <BALL/VIEW/KERNEL/controllers/applyPayload.h>
#endif

#ifndef BALL_VIEW_KERNEL_STAGE_H
# include <BALL/VIEW/KERNEL/stage.h>
#endif

#include <QtCore/QObject>

#include <vector>

// ---------------------------------------------------------------------------
// 999.59-03 per-controller contract-fixture activation symbol (Codex MEDIUM
// #7) — flips light_contract_test's parity block live (see
// test/contract/contractFixtureActivation.h). Defined ONLY because apply()
// below returns bool. §2a declared invalidation: SOFT refresh (a light change
// re-lights existing geometry; the primitive set is unchanged).
// ---------------------------------------------------------------------------
#define BALL_VIEW_LIGHT_APPLY_BOOL 1

namespace BALL
{
	namespace VIEW
	{
		class Stage;

		/**
		 * Lighting controller. Owns the COMPLETE per-light list (the
		 * authoritative lights_ vector the legacy LightSettings dialog
		 * held) plus the aggregate ambient-intensity slider value.
		 *
		 * apply() mirrors LightSettings::apply()
		 * (source/VIEW/DIALOGS/lightSettings.C:384-393): clear the Stage
		 * light sources and re-add the controller's authoritative list,
		 * then Scene::lightsUpdated(true). An AMBIENT default is injected
		 * for an empty list so existing rendered scenes are unchanged
		 * (mirrors defaultsPressed()).
		 *
		 * revert() / syncFromStage() pull the full live light list from
		 * the Stage (mirrors LightSettings::updateFromStage,
		 * lightSettings.C:69-86). syncFromStage() is the public entry
		 * point the Scene render-refresh / camera-move call sites use in
		 * place of the legacy light_settings_->updateFromStage().
		 */
		class BALL_VIEW_EXPORT LightController : public Controller
		{
			Q_OBJECT

			Q_PROPERTY(int lightCount READ lightCount NOTIFY lightCountChanged)
			Q_PROPERTY(float ambientIntensity READ ambientIntensity WRITE setAmbientIntensity NOTIFY ambientIntensityChanged)

			public:
				explicit LightController(Stage* stage = nullptr,
				                         QObject* parent = nullptr);
				~LightController() override;

				Stage* stage() const { return stage_; }
				void setStage(Stage* stage);

				int   lightCount()       const { return light_count_; }
				float ambientIntensity() const { return ambient_intensity_; }

				/// The controller's authoritative full light list (the
				/// vector LightSettings::lights_ used to hold). apply()
				/// pushes this to the Stage; syncFromStage() refills it.
				const std::vector<LightSource>& getLightSources() const { return lights_; }
				void setLightSources(const std::vector<LightSource>& lights);

				/// v1.7.x-24 — true while apply() is mutating the Stage.
				/// 999.59-03 — re-entrancy state now lives in the base depth
				/// counter (applying_depth_).
				bool isApplying() const { return applying_depth_ > 0; }

				/**
				 * Apply the staged light list (clear + re-add + ambient) to the
				 * attached Stage, command-shaped per §2. Returns true when the
				 * Stage was mutated, false when dropped (re-entry) or rejected
				 * (no Stage / busy). Overrides Controller::apply(); called
				 * directly (controller_->apply()), not as a string-based slot.
				 * @return true if the owner was mutated, false otherwise.
				 */
				bool apply() override;

				/**
				 * Reset the mirror to the owner's current state (999.64 reset
				 * path). Overrides Controller::reset(); delegates to revert().
				 */
				void reset() override;

			public Q_SLOTS:
				void revert();

				/// Public stage-resync entry point. Re-reads the full live
				/// light list from the Stage into the controller (mirrors
				/// LightSettings::updateFromStage). The Scene camera-move /
				/// render-refresh call sites call this in place of the
				/// legacy light_settings_->updateFromStage() (999.58-01).
				/// Implemented as a thin alias of revert().
				void syncFromStage();

				void setAmbientIntensity(float v);

			Q_SIGNALS:
				void lightCountChanged(int c);
				void ambientIntensityChanged(float v);
				void appliedStub();

			protected:
				/**
				 * §2a declared invalidation — SOFT refresh: a light-list change
				 * re-lights the EXISTING geometry via Scene::lightsUpdated(true)
				 * without rebuilding display lists. Overrides
				 * Controller::invalidateDeclared_().
				 */
				void invalidateDeclared_() override;

			private:
				/// §13-cookbook mutation body, moved out of apply(): clears the
				/// Stage lights and re-adds the controller's authoritative list
				/// plus the aggregate ambient. Precondition (stage_ != nullptr)
				/// guaranteed by apply().
				bool applyInternal_();

				/// Capture-only reversible intent (§2 step 7; v2.0 UndoStack).
				void recordIntent_(const ApplyPayload& payload);

				Stage* stage_;
				int    light_count_;
				float  ambient_intensity_;

				// Authoritative full per-light list (position / direction /
				// type / intensity / color / attenuation / relative-to-camera)
				// — the same role LightSettings::lights_ played. apply() clears
				// the Stage and re-adds this list.
				std::vector<LightSource> lights_;

				ApplyPayload last_payload_;  // 999.59-03 — capture-only reversible intent.
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_KERNEL_CONTROLLERS_LIGHTCONTROLLER_H
