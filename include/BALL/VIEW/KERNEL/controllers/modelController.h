// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 Plan 04 — ModelController (read-only mirror).
//
// Source dialog: source/VIEW/DIALOGS/modelSettingsDialog.C (~385 LOC).
// Mutation cut-over deferred per Plan 06 (StageController cut-over
// proof-of-concept first; the rest follow the same shape).
//

#ifndef BALL_VIEW_KERNEL_CONTROLLERS_MODELCONTROLLER_H
#define BALL_VIEW_KERNEL_CONTROLLERS_MODELCONTROLLER_H


#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#include <QtCore/QObject>
#include <QtCore/QString>

namespace BALL
{
	namespace VIEW
	{
		class Representation;

		/**
		 * Presentation controller for per-Representation Model
		 * settings: model type / drawing precision / drawing mode /
		 * transparency. Read-only mirror in this plan; mutation
		 * cut-over follows the StageController pattern in a later
		 * plan.
		 */
		class BALL_VIEW_EXPORT ModelController : public QObject
		{
			Q_OBJECT

			Q_PROPERTY(int modelType READ modelType WRITE setModelType NOTIFY modelTypeChanged)
			Q_PROPERTY(int drawingMode READ drawingMode WRITE setDrawingMode NOTIFY drawingModeChanged)
			Q_PROPERTY(int drawingPrecision READ drawingPrecision WRITE setDrawingPrecision NOTIFY drawingPrecisionChanged)
			Q_PROPERTY(int transparency READ transparency WRITE setTransparency NOTIFY transparencyChanged)
			Q_PROPERTY(float ballRadius READ ballRadius WRITE setBallRadius NOTIFY ballRadiusChanged)
			Q_PROPERTY(float ballStickBondRadius READ ballStickBondRadius WRITE setBallStickBondRadius NOTIFY ballStickBondRadiusChanged)
			Q_PROPERTY(bool dashedBonds READ dashedBonds WRITE setDashedBonds NOTIFY dashedBondsChanged)
			Q_PROPERTY(float stickRadius READ stickRadius WRITE setStickRadius NOTIFY stickRadiusChanged)
			Q_PROPERTY(float surfaceProbeRadius READ surfaceProbeRadius WRITE setSurfaceProbeRadius NOTIFY surfaceProbeRadiusChanged)
			Q_PROPERTY(float cartoonTubeRadius READ cartoonTubeRadius WRITE setCartoonTubeRadius NOTIFY cartoonTubeRadiusChanged)

			public:
				explicit ModelController(Representation* rep = nullptr,
				                         QObject* parent = nullptr);
				~ModelController() override;

				Representation* representation() const { return rep_; }
				void setRepresentation(Representation* rep);

				int modelType() const        { return model_type_; }
				int drawingMode() const      { return drawing_mode_; }
				int drawingPrecision() const { return drawing_precision_; }
				int transparency() const     { return transparency_; }
				bool isApplying() const      { return applying_; }

				float ballRadius() const          { return ball_radius_; }
				float ballStickBondRadius() const { return ball_stick_bond_radius_; }
				bool  dashedBonds() const         { return dashed_bonds_; }
				float stickRadius() const         { return stick_radius_; }
				float surfaceProbeRadius() const  { return surface_probe_radius_; }
				float cartoonTubeRadius() const   { return cartoon_tube_radius_; }

			public Q_SLOTS:
				void apply();
				void revert();

				void setModelType(int t);
				void setDrawingMode(int m);
				void setDrawingPrecision(int p);
				void setTransparency(int t);

				void setBallRadius(float v);
				void setBallStickBondRadius(float v);
				void setDashedBonds(bool v);
				void setStickRadius(float v);
				void setSurfaceProbeRadius(float v);
				void setCartoonTubeRadius(float v);

			Q_SIGNALS:
				void modelTypeChanged(int t);
				void drawingModeChanged(int m);
				void drawingPrecisionChanged(int p);
				void transparencyChanged(int t);
				void ballRadiusChanged(float v);
				void ballStickBondRadiusChanged(float v);
				void dashedBondsChanged(bool v);
				void stickRadiusChanged(float v);
				void surfaceProbeRadiusChanged(float v);
				void cartoonTubeRadiusChanged(float v);
				void appliedStub();

			private:
				Representation* rep_;
				int model_type_;
				int drawing_mode_;
				int drawing_precision_;
				int transparency_;
				float ball_radius_;
				float ball_stick_bond_radius_;
				bool  dashed_bonds_;
				float stick_radius_;
				float surface_probe_radius_;
				float cartoon_tube_radius_;
				bool  params_dirty_;
				bool  applying_;  // v1.7.x-24 — re-entrancy shield (see ControllerApplyGuard).
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_KERNEL_CONTROLLERS_MODELCONTROLLER_H
