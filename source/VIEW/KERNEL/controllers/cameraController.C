// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 Plan 05 — CameraController implementation.
// UFG-11 cut-over (v1.7-modernization) — apply() now mutates the
// attached Stage's Camera (viewPoint + lookAt) and triggers a Scene
// redraw, mirroring the legacy SetCamera::okPressed() backend. Same
// pattern as StageController / MaterialController cut-overs.
//

#include <BALL/VIEW/KERNEL/controllers/cameraController.h>


#include <BALL/VIEW/KERNEL/stage.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/WIDGETS/scene.h>
#include <BALL/VIEW/RENDERING/camera.h>
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

			Vector3 fromQ_(const QVector3D& v)
			{
				return Vector3(static_cast<float>(v.x()),
				               static_cast<float>(v.y()),
				               static_cast<float>(v.z()));
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
			// UFG-11 cut-over — push mirrored viewPoint + lookAt to the
			// attached Stage's Camera. Same backend the legacy SetCamera
			// modal mutates (see source/VIEW/DIALOGS/setCamera.C:87-89).
			// Inspector's CameraSection in v1.7 is read-only display, but
			// apply() is wired for future write paths and for programmatic
			// callers (scripts, tests).
			if (stage_ == nullptr)
			{
				Log.warn() << "[CameraController::apply] no Stage attached — skipping." << std::endl;
				return;
			}

			MainControl* mc = MainControl::getInstance(0);
			if (mc != nullptr && mc->isBusy())
			{
				Log.info() << "[CameraController::apply] MainControl busy — deferring." << std::endl;
				return;
			}

			// Reject degenerate camera state (viewpoint == lookAt) — would
			// produce a zero-length view vector and a black render.
			const Vector3 vp = fromQ_(position_);
			const Vector3 lp = fromQ_(look_at_);
			if (vp == lp)
			{
				Log.error() << "[CameraController::apply] viewPoint == lookAt — refusing to apply." << std::endl;
				return;
			}

			Camera& cam = stage_->getCamera();
			cam.setViewPoint(vp);
			cam.setLookAtPosition(lp);

			// Trigger a redraw through the live Scene. Materials/Stage
			// controllers route through Scene::getInstance(0); same hook
			// here keeps the cut-over consistent.
			Scene* scene = Scene::getInstance(0);
			if (scene != nullptr)
			{
				scene->update();
			}

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

