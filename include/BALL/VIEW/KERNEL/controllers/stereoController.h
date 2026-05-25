// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 Plan 05 — StereoController (read-only mirror).
//
// Source dialog: source/VIEW/DIALOGS/stereoSettingsDialog.C (~159 LOC).
//

#ifndef BALL_VIEW_KERNEL_CONTROLLERS_STEREOCONTROLLER_H
#define BALL_VIEW_KERNEL_CONTROLLERS_STEREOCONTROLLER_H


#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#ifndef BALL_VIEW_KERNEL_CONTROLLERS_CONTROLLER_H
# include <BALL/VIEW/KERNEL/controllers/controller.h>
#endif

#ifndef BALL_VIEW_KERNEL_CONTROLLERS_APPLYPAYLOAD_H
# include <BALL/VIEW/KERNEL/controllers/applyPayload.h>
#endif

#include <QtCore/QObject>

// ---------------------------------------------------------------------------
// 999.59-03 per-controller contract-fixture activation symbol (Codex MEDIUM
// #7) — flips stereo_contract_test's parity block live (see
// test/contract/contractFixtureActivation.h). Defined ONLY because apply()
// below returns bool. §2a declared invalidation: SOFT refresh (an eye/focal
// distance change re-projects existing geometry; the primitive set is the same).
// ---------------------------------------------------------------------------
#define BALL_VIEW_STEREO_APPLY_BOOL 1

namespace BALL
{
	namespace VIEW
	{
		class Stage;

		/**
		 * Stereo-rendering mode + eye/focal distances. Cut over to the §2
		 * `bool apply()` command contract in 999.59-03 (declared §2a
		 * invalidation: SOFT refresh — an eye/focal change leaves the
		 * primitive set intact).
		 */
		class BALL_VIEW_EXPORT StereoController : public Controller
		{
			Q_OBJECT

			Q_PROPERTY(bool stereoEnabled READ stereoEnabled WRITE setStereoEnabled NOTIFY stereoEnabledChanged)
			Q_PROPERTY(float eyeDistance READ eyeDistance WRITE setEyeDistance NOTIFY eyeDistanceChanged)
			Q_PROPERTY(float focalDistance READ focalDistance WRITE setFocalDistance NOTIFY focalDistanceChanged)
			Q_PROPERTY(bool swapSideBySide READ swapSideBySide WRITE setSwapSideBySide NOTIFY swapSideBySideChanged)

			public:
				explicit StereoController(Stage* stage = nullptr,
				                          QObject* parent = nullptr);
				~StereoController() override;

				Stage* stage() const { return stage_; }
				void setStage(Stage* stage);

				bool  stereoEnabled()  const { return enabled_; }
				float eyeDistance()    const { return eye_distance_; }
				float focalDistance()  const { return focal_distance_; }
				bool  swapSideBySide() const { return swap_sbs_; }

				/// v1.7.x-24 — true while apply() is mutating the Stage.
				/// 999.59-03 — re-entrancy state now lives in the base depth
				/// counter (applying_depth_).
				bool isApplying() const { return applying_depth_ > 0; }

				/**
				 * Push the mirrored eye/focal distance + swap-side-by-side to
				 * the attached Stage, command-shaped per §2. Returns true when
				 * the Stage was mutated, false when dropped (re-entry) or
				 * rejected (no Stage / busy). Overrides Controller::apply();
				 * called directly (controller_->apply()), not as a string-based
				 * slot.
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
				void setStereoEnabled(bool b);
				void setEyeDistance(float v);
				void setFocalDistance(float v);
				void setSwapSideBySide(bool b);

			Q_SIGNALS:
				void stereoEnabledChanged(bool b);
				void eyeDistanceChanged(float v);
				void focalDistanceChanged(float v);
				void swapSideBySideChanged(bool b);
				void appliedStub();

			protected:
				/**
				 * §2a declared invalidation — SOFT refresh: an eye/focal/swap
				 * change re-projects the EXISTING geometry via Scene::update()
				 * without rebuilding display lists. Overrides
				 * Controller::invalidateDeclared_().
				 */
				void invalidateDeclared_() override;

			private:
				/// §13-cookbook mutation body, moved out of apply(): writes
				/// eye/focal distance + swap-side-by-side onto the Stage.
				/// Precondition (stage_ != nullptr) guaranteed by apply().
				bool applyInternal_();

				/// Capture-only reversible intent (§2 step 7; v2.0 UndoStack).
				void recordIntent_(const ApplyPayload& payload);

				Stage* stage_;
				bool   enabled_;
				float  eye_distance_;
				float  focal_distance_;
				bool   swap_sbs_;

				ApplyPayload last_payload_;  // 999.59-03 — capture-only reversible intent.
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_KERNEL_CONTROLLERS_STEREOCONTROLLER_H
