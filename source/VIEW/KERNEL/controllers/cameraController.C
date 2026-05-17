// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 Plan 05 — CameraController implementation.
//

#include <BALL/VIEW/KERNEL/controllers/cameraController.h>

#ifdef BALL_UI_V2

#include <BALL/VIEW/KERNEL/stage.h>
#include <BALL/COMMON/logStream.h>

namespace BALL
{
	namespace VIEW
	{

		namespace
		{
			QVector3D toQ_(const Vector3& v)
			{
				return QVector3D(static_cast<float>(v.x),
				                 static_cast<float>(v.y),
				                 static_cast<float>(v.z));
			}
		}

		CameraController::CameraController(Stage* stage, QObject* parent)
			: QObject(parent), stage_(stage)
		{
			revert();
		}

		CameraController::~CameraController() = default;

		void CameraController::setStage(Stage* stage)
		{
			if (stage_ == stage) return;
			stage_ = stage;
			revert();
		}

		void CameraController::revert()
		{
			if (stage_ == nullptr) return;
			QVector3D p = toQ_(stage_->getCamera().getViewPoint());
			QVector3D l = toQ_(stage_->getCamera().getLookAtPosition());
			if (p != position_) { position_ = p; Q_EMIT positionChanged(p); }
			if (l != look_at_)  { look_at_  = l; Q_EMIT lookAtChanged(l); }
		}

		void CameraController::apply()
		{
			Log.info() << "[CameraController::apply] STUB — legacy SetCamera dialog "
				"owns mutation. Mirrored viewPoint=("
				<< position_.x() << "," << position_.y() << "," << position_.z()
				<< ") lookAt=(" << look_at_.x() << "," << look_at_.y() << "," << look_at_.z()
				<< ")" << std::endl;
			Q_EMIT appliedStub();
		}

		void CameraController::setPosition(const QVector3D& p)
		{
			if (p == position_) return;
			position_ = p;
			Q_EMIT positionChanged(p);
		}

		void CameraController::setLookAt(const QVector3D& p)
		{
			if (p == look_at_) return;
			look_at_ = p;
			Q_EMIT lookAtChanged(p);
		}

	} // namespace VIEW
} // namespace BALL

#endif // BALL_UI_V2
