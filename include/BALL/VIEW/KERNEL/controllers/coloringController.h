// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 Plan 04 — ColoringController (read-only mirror).
//
// Source dialog: source/VIEW/DIALOGS/coloringSettingsDialog.C (~878 LOC).
//

#ifndef BALL_VIEW_KERNEL_CONTROLLERS_COLORINGCONTROLLER_H
#define BALL_VIEW_KERNEL_CONTROLLERS_COLORINGCONTROLLER_H


#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#include <QtCore/QObject>

namespace BALL
{
	namespace VIEW
	{
		class Representation;

		/**
		 * Per-Representation Coloring settings — the coloringMethod
		 * enum + any per-method parameters. Read-only mirror for the
		 * primary coloring-method enum in this plan; per-method
		 * parameter mirrors follow in the cut-over plan.
		 */
		class BALL_VIEW_EXPORT ColoringController : public QObject
		{
			Q_OBJECT

			Q_PROPERTY(int coloringMethod READ coloringMethod WRITE setColoringMethod NOTIFY coloringMethodChanged)
			Q_PROPERTY(float valueMin READ valueMin WRITE setValueMin NOTIFY valueMinChanged)
			Q_PROPERTY(float valueMax READ valueMax WRITE setValueMax NOTIFY valueMaxChanged)

			public:
				explicit ColoringController(Representation* rep = nullptr,
				                            QObject* parent = nullptr);
				~ColoringController() override;

				Representation* representation() const { return rep_; }
				void setRepresentation(Representation* rep);

				int coloringMethod() const { return coloring_method_; }
				float valueMin() const { return value_min_; }
				float valueMax() const { return value_max_; }
				bool isApplying() const { return applying_; }

			public Q_SLOTS:
				void apply();
				void revert();
				void setColoringMethod(int m);
				void setValueMin(float v);
				void setValueMax(float v);

			Q_SIGNALS:
				void coloringMethodChanged(int m);
				void valueMinChanged(float v);
				void valueMaxChanged(float v);
				void appliedStub();

			private:
				Representation* rep_;
				int coloring_method_;
				float value_min_;
				float value_max_;
				bool applying_;  // v1.7.x-24 — re-entrancy shield (see ControllerApplyGuard).
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_KERNEL_CONTROLLERS_COLORINGCONTROLLER_H
