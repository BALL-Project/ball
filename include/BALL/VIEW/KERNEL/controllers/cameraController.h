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

#include <QtCore/QObject>
#include <QtGui/QVector3D>

namespace BALL
{
	namespace VIEW
	{
		class Stage;

		/**
		 * Mirrors the Camera state attached to a Stage: position +
		 * look-at + view-vector. Read-only mirror — the legacy
		 * SetCamera dialog still owns mutation. Cut-over plan binds
		 * apply() through the Renderer / RenderSurface boundary per
		 * SEED-001 step 5.
		 */
		class BALL_VIEW_EXPORT CameraController : public QObject
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

			public Q_SLOTS:
				void apply();
				void revert();
				void setPosition(const QVector3D& p);
				void setLookAt(const QVector3D& p);

			Q_SIGNALS:
				void positionChanged(const QVector3D& p);
				void lookAtChanged(const QVector3D& p);
				void appliedStub();

			private:
				Stage* stage_;
				QVector3D position_;
				QVector3D look_at_;
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_KERNEL_CONTROLLERS_CAMERACONTROLLER_H
