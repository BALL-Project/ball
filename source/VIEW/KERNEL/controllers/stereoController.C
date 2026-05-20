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
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/WIDGETS/scene.h>
#include <BALL/VIEW/KERNEL/controllers/controllerApplyGuard.h>
#include <BALL/COMMON/logStream.h>

namespace BALL
{
	namespace VIEW
	{

		StereoController::StereoController(Stage* stage, QObject* parent)
			: QObject(parent), stage_(stage),
				enabled_(false),
				eye_distance_(0.0f),
				focal_distance_(0.0f),
				swap_sbs_(false),
				applying_(false)
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

		void StereoController::apply()
		{
			// UFG-11 cut-over — push mirrored eye/focal distance +
			// swap-side-by-side flag to the attached Stage. Mirrors the
			// legacy StereoSettingsDialog::apply() backend
			// (stereoSettingsDialog.C:109-115) and StageSettings::apply()
			// stereo-swap mutation (stageSettings.C:254). enabled_ is
			// renderer-mode state — stays a controller-side mirror until
			// SEED-001 step 5 wires StereoController through the
			// Renderer / RenderSurface boundary.
			if (stage_ == nullptr)
			{
				Log.warn() << "[StereoController::apply] no Stage attached — skipping." << std::endl;
				return;
			}

			MainControl* mc = MainControl::getInstance(0);
			if (mc != nullptr && mc->isBusy())
			{
				Log.info() << "[StereoController::apply] MainControl busy — deferring." << std::endl;
				return;
			}

			// v1.7.x-24 — re-entrancy shield. If a notification triggered by
			// this apply() (e.g. the scene update below) synchronously
			// re-enters apply(), bail rather than re-running the mutation —
			// this is the cascade class behind the v1.7.x-13 freeze. The RAII
			// guard clears the flag on every exit path.
			if (applying_) return;
			ControllerApplyGuard apply_guard(applying_);

			stage_->setEyeDistance(eye_distance_);
			stage_->setFocalDistance(focal_distance_);
			stage_->setSwapSideBySideStereo(swap_sbs_);

			Scene* scene = Scene::getInstance(0);
			if (scene != nullptr)
			{
				scene->update();
			}

			Q_EMIT appliedStub();
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

