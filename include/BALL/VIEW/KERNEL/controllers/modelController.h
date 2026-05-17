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

#ifdef BALL_UI_V2

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

			public Q_SLOTS:
				void apply();
				void revert();

				void setModelType(int t);
				void setDrawingMode(int m);
				void setDrawingPrecision(int p);
				void setTransparency(int t);

			Q_SIGNALS:
				void modelTypeChanged(int t);
				void drawingModeChanged(int m);
				void drawingPrecisionChanged(int p);
				void transparencyChanged(int t);
				void appliedStub();

			private:
				Representation* rep_;
				int model_type_;
				int drawing_mode_;
				int drawing_precision_;
				int transparency_;
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_UI_V2
#endif // BALL_VIEW_KERNEL_CONTROLLERS_MODELCONTROLLER_H
