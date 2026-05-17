// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 Plan 05 — StereoController implementation.
//

#include <BALL/VIEW/KERNEL/controllers/stereoController.h>

#ifdef BALL_UI_V2

#include <BALL/VIEW/KERNEL/stage.h>
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

		void StereoController::apply()
		{
			Log.info() << "[StereoController::apply] STUB — legacy StereoSettingsDialog "
				"owns mutation. enabled=" << (enabled_ ? "true" : "false")
				<< " eye=" << eye_distance_
				<< " focal=" << focal_distance_
				<< " swapSBS=" << (swap_sbs_ ? "true" : "false") << std::endl;
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

#endif // BALL_UI_V2
