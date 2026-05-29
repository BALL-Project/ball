// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 Plan 05 — CameraController (read-only mirror).
//
// Source dialog: source/VIEW/DIALOGS/setCamera.C (~94 LOC).
//

#ifndef BALL_VIEW_KERNEL_CONTROLLERS_CAMERACONTROLLER_H
#define BALL_VIEW_KERNEL_CONTROLLERS_CAMERACONTROLLER_H


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
#include <QtGui/QVector3D>

// ---------------------------------------------------------------------------
// 999.59-03 per-controller contract-fixture activation symbol (Codex MEDIUM
// #7) — flips camera_contract_test's parity block live (see
// test/contract/contractFixtureActivation.h). Defined ONLY because apply()
// below returns bool. §2a declared invalidation: SOFT refresh (a camera
// transform change re-reads the view; the primitive set is unchanged).
// ---------------------------------------------------------------------------
#define BALL_VIEW_CAMERA_APPLY_BOOL 1

namespace BALL
{
	namespace VIEW
	{
		class Stage;

		/**
		 * Mirrors the Camera state attached to a Stage: position +
		 * look-at + view-vector. Cut over to the §2 `bool apply()`
		 * command contract in 999.59-03 (declared §2a invalidation: SOFT
		 * refresh — a transform change leaves the primitive set intact).
		 */
		class BALL_VIEW_EXPORT CameraController : public Controller
		{
			Q_OBJECT

			Q_PROPERTY(QVector3D position READ position WRITE setPosition NOTIFY positionChanged)
			Q_PROPERTY(QVector3D lookAt READ lookAt WRITE setLookAt NOTIFY lookAtChanged)

			public:
				explicit CameraController(Stage* stage = nullptr,
				                          QObject* parent = nullptr);
				~CameraController() override;

				Stage* stage() const { return stage_; }
				void setStage(Stage* stage);

				QVector3D position() const { return position_; }
				QVector3D lookAt()   const { return look_at_; }

				/// v1.7.x-24 — true while apply() is mutating the Stage.
				/// 999.59-03 — re-entrancy state now lives in the base depth
				/// counter (applying_depth_).
				bool isApplying() const { return applying_depth_ > 0; }

				/**
				 * Push the mirrored viewPoint + lookAt to the attached Stage's
				 * Camera, command-shaped per §2. Returns true when the Camera
				 * was mutated, false when dropped (re-entry) or rejected (no
				 * Stage / busy / degenerate camera). Overrides
				 * Controller::apply(); called directly (controller_->apply()),
				 * not as a string-based Qt slot.
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
				void setPosition(const QVector3D& p);
				void setLookAt(const QVector3D& p);

			Q_SIGNALS:
				void positionChanged(const QVector3D& p);
				void lookAtChanged(const QVector3D& p);
				void appliedStub();

			protected:
				/**
				 * §2a declared invalidation — SOFT refresh: a camera transform
				 * change re-reads the view through the live Scene without
				 * rebuilding display lists. Overrides
				 * Controller::invalidateDeclared_().
				 */
				void invalidateDeclared_() override;

			private:
				/// §13-cookbook mutation body, moved out of apply(): writes
				/// viewPoint + lookAt onto the Stage camera. Precondition
				/// (stage_ != nullptr, non-degenerate) guaranteed by apply().
				bool applyInternal_();

				/// Capture-only reversible intent (§2 step 7; v2.0 UndoStack).

				Stage* stage_;
				QVector3D position_;
				QVector3D look_at_;

		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_KERNEL_CONTROLLERS_CAMERACONTROLLER_H
