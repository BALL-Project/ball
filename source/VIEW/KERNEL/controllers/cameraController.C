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

			// Compact owner-state snapshot for the ApplyPayload (§2 step 3).
			String snapshotCameraState_(Stage* stage)
			{
				if (stage == nullptr) return String("camera{null}");
				std::ostringstream s;
				const Vector3& vp = stage->getCamera().getViewPoint();
				const Vector3& la = stage->getCamera().getLookAtPosition();
				s << "vp=" << vp.x << ',' << vp.y << ',' << vp.z
				  << ";la=" << la.x << ',' << la.y << ',' << la.z;
				return String(s.str());
			}
		}

		CameraController::CameraController(Stage* stage, QObject* parent)
			: Controller(parent), stage_(stage)
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

		bool CameraController::apply()
		{
			// 999.59-03 cut-over to the §2 `bool apply()` command contract.
			// Push mirrored viewPoint + lookAt to the attached Stage's Camera.
			// Same backend the legacy SetCamera modal mutates (see
			// source/VIEW/DIALOGS/setCamera.C:87-89) and the §3a interactive
			// site scene.C::restoreViewPoint() now routes through.

			// ── 1. Preconditions (§2 step 1) ─────────────────────────────────
			if (stage_ == nullptr)
			{
				Log.warn() << "[CameraController::apply] no Stage attached — skipping." << std::endl;
				return false;                    // rejected — nothing to mutate
			}

			MainControl* mc = MainControl::getInstance(0);
			if (mc != nullptr && mc->isBusy())
			{
				Log.info() << "[CameraController::apply] MainControl busy — deferring." << std::endl;
				return false;                    // rejected — busy
			}

			// Reject degenerate camera state (viewpoint == lookAt) — would
			// produce a zero-length view vector and a black render.
			const Vector3 vp = fromQ_(position_);
			const Vector3 lp = fromQ_(look_at_);
			if (vp == lp)
			{
				Log.error() << "[CameraController::apply] viewPoint == lookAt — refusing to apply." << std::endl;
				return false;                    // rejected — validation
			}

			// ── 2. Re-entrancy shield (§2 step 2 / §4) — nest-aware drop ─────
			if (applying_depth_ > 0) return false;          // dropped — re-entry
			ControllerApplyGuard apply_guard(applying_depth_);

			// ── 3. Capture reversible intent (§2 step 3) ─────────────────────
			ApplyPayload payload;
			payload.command_id = "camera.setCamera";
			payload.before     = snapshotCameraState_(stage_);
			payload.target     = stage_;
			payload.t_us       = PreciseTime::now().getMicroSeconds();

			// ── 4. Mutate the single owner (§2 step 4) ───────────────────────
			applyInternal_();

			payload.after = snapshotCameraState_(stage_);

			// ── 5. Emit ONE typed event (§2 step 5) ──────────────────────────
			Q_EMIT appliedStub();

			// ── 6. Request the DECLARED invalidation (§2 step 6 / §2a) ───────
			invalidateDeclared_();               // soft refresh

			// ── 7. Record reversible intent (§2 step 7) — capture only ───────
			recordIntent_(payload);

			return true;                          // mutated
		}

		bool CameraController::applyInternal_()
		{
			// §13 cookbook step 2 — mutation body, moved out of apply().
			// Precondition (stage_ != nullptr, non-degenerate) guaranteed by
			// apply().
			Camera& cam = stage_->getCamera();
			cam.setViewPoint(fromQ_(position_));
			cam.setLookAtPosition(fromQ_(look_at_));
			return true;
		}

		void CameraController::invalidateDeclared_()
		{
			// §2a Camera row — SOFT refresh. A camera transform change re-reads
			// the view through the live Scene without rebuilding display lists
			// (the primitive set is unchanged). Materials/Stage controllers
			// route through Scene::getInstance(0); same hook here keeps the
			// cut-over consistent.
			Scene* scene = Scene::getInstance(0);
			if (scene != nullptr)
			{
				scene->update();
			}
		}

		void CameraController::reset()
		{
			// §2 reset path (999.64 consumes) — single-pass re-sync from owner.
			revert();
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

