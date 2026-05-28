// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 Plan 05 — StereoController implementation.
// UFG-11 cut-over (v1.7-modernization) — apply() now writes
// eye/focal distance + swap-side-by-side to the attached Stage and
// triggers a Scene redraw, mirroring the legacy
// StereoSettingsDialog::apply() backend
// (source/VIEW/DIALOGS/stereoSettingsDialog.C:109-115) and the
// stereo-swap mutation in StageSettings::apply()
// (source/VIEW/DIALOGS/stageSettings.C:254). The "enabled" toggle is
// renderer-mode state owned by Scene's stereo plumbing
// (Renderer::setStereoMode); we mirror the field but leave the actual
// renderer-mode switch to the legacy Display › Stereo menu entry
// until the StereoController/Renderer-interface binding lands per
// SEED-001 step 5.
//

#include <BALL/VIEW/KERNEL/controllers/stereoController.h>


#include <BALL/VIEW/KERNEL/stage.h>
#include <BALL/VIEW/KERNEL/stageMutation.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/WIDGETS/scene.h>
#include <BALL/VIEW/KERNEL/controllers/controllerApplyGuard.h>
#include <BALL/COMMON/logStream.h>
#include <BALL/CONCEPT/timeStamp.h>

#include <sstream>

namespace BALL
{
	namespace VIEW
	{

		namespace
		{
			// Compact owner-state snapshot for the ApplyPayload (§2 step 3).
			String snapshotStereoState_(Stage* stage)
			{
				if (stage == nullptr) return String("stereo{null}");
				std::ostringstream s;
				s << "eye=" << stage->getEyeDistance()
				  << ";focal=" << stage->getFocalDistance()
				  << ";swap=" << (stage->swapSideBySideStereo() ? 1 : 0);
				return String(s.str());
			}
		}

		StereoController::StereoController(Stage* stage, QObject* parent)
			: Controller(parent), stage_(stage),
				enabled_(false),
				eye_distance_(0.0f),
				focal_distance_(0.0f),
				swap_sbs_(false)
		{
			revert();
		}

		StereoController::~StereoController() = default;

		void StereoController::setStage(Stage* stage)
		{
			if (stage_ == stage) return;
			stage_ = stage;
			revert();
		}

		void StereoController::revert()
		{
			if (stage_ == nullptr) return;
			float ed = stage_->getEyeDistance();
			if (ed != eye_distance_) { eye_distance_ = ed; Q_EMIT eyeDistanceChanged(ed); }
			float fd = stage_->getFocalDistance();
			if (fd != focal_distance_) { focal_distance_ = fd; Q_EMIT focalDistanceChanged(fd); }
			bool swap = stage_->swapSideBySideStereo();
			if (swap != swap_sbs_) { swap_sbs_ = swap; Q_EMIT swapSideBySideChanged(swap); }
		}

		bool StereoController::apply()
		{
			// 999.59-03 cut-over to the §2 `bool apply()` command contract.
			// Push mirrored eye/focal distance + swap-side-by-side flag to the
			// attached Stage. Mirrors the legacy StereoSettingsDialog::apply()
			// backend (stereoSettingsDialog.C:109-115) — the §3a interactive
			// sites (stereoSettingsDialog + interactionModeManager eye/focal
			// adjust) now route through this controller. enabled_ is
			// renderer-mode state — stays a controller-side mirror until
			// SEED-001 step 5 wires StereoController through the
			// Renderer / RenderSurface boundary.

			// ── 1. Preconditions (§2 step 1) ─────────────────────────────────
			if (stage_ == nullptr)
			{
				Log.warn() << "[StereoController::apply] no Stage attached — skipping." << std::endl;
				return false;                    // rejected — nothing to mutate
			}

			MainControl* mc = MainControl::getInstance(0);
			if (mc != nullptr && mc->isBusy())
			{
				Log.info() << "[StereoController::apply] MainControl busy — deferring." << std::endl;
				return false;                    // rejected — busy
			}

			// ── 2. Re-entrancy shield (§2 step 2 / §4) — nest-aware drop ─────
			if (applying_depth_ > 0) return false;          // dropped — re-entry
			ControllerApplyGuard apply_guard(applying_depth_);

			// ── 3. Capture reversible intent (§2 step 3) ─────────────────────
			ApplyPayload payload;
			payload.command_id = "stereo.setEyeDistance";
			payload.before     = snapshotStereoState_(stage_);
			payload.target     = stage_;
			payload.t_us       = PreciseTime::now().getMicroSeconds();

			// ── 4. Mutate the single owner (§2 step 4) ───────────────────────
			applyInternal_();

			payload.after = snapshotStereoState_(stage_);

			// ── 5. Emit ONE typed event (§2 step 5) ──────────────────────────
			Q_EMIT appliedStub();

			// ── 6. Request the DECLARED invalidation (§2 step 6 / §2a) ───────
			invalidateDeclared_();               // soft refresh

			// ── 7. Record reversible intent (§2 step 7) — capture only ───────
			recordIntent_(payload);

			return true;                          // mutated
		}

		bool StereoController::applyInternal_()
		{
			// §13 cookbook step 2 — mutation body, moved out of apply().
			// Precondition (stage_ != nullptr) guaranteed by apply().
			// §3b — Stage mutation flows through the single StageMutation friend.
			StageMutation(*stage_)
				.eyeDistance(eye_distance_)
				.focalDistance(focal_distance_);
			stage_->setSwapSideBySideStereo(swap_sbs_);
			return true;
		}

		void StereoController::invalidateDeclared_()
		{
			// §2a Stereo row — SOFT refresh. An eye/focal/swap change
			// re-projects the EXISTING geometry via Scene::update(); the
			// primitive set is unchanged, so no display-list rebuild is needed.
			Scene* scene = Scene::getInstance(0);
			if (scene != nullptr)
			{
				scene->update();
			}
		}

		void StereoController::reset()
		{
			// §2 reset path (999.64 consumes) — single-pass re-sync from owner.
			revert();
		}

		void StereoController::recordIntent_(const ApplyPayload& payload)
		{
			// v1.7.4 — capture only (no UndoStack yet, §2 step 7).
			last_payload_ = payload;
		}

		void StereoController::setStereoEnabled(bool b)
		{
			if (b == enabled_) return;
			enabled_ = b;
			Q_EMIT stereoEnabledChanged(b);
		}

		void StereoController::setEyeDistance(float v)
		{
			if (v == eye_distance_) return;
			eye_distance_ = v;
			Q_EMIT eyeDistanceChanged(v);
		}

		void StereoController::setFocalDistance(float v)
		{
			if (v == focal_distance_) return;
			focal_distance_ = v;
			Q_EMIT focalDistanceChanged(v);
		}

		void StereoController::setSwapSideBySide(bool b)
		{
			if (b == swap_sbs_) return;
			swap_sbs_ = b;
			Q_EMIT swapSideBySideChanged(b);
		}

	} // namespace VIEW
} // namespace BALL

