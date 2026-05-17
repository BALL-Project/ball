// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 Plan 04 — ColoringController (read-only mirror).
//
// Source dialog: source/VIEW/DIALOGS/coloringSettingsDialog.C (~878 LOC).
//

#ifndef BALL_VIEW_KERNEL_CONTROLLERS_COLORINGCONTROLLER_H
#define BALL_VIEW_KERNEL_CONTROLLERS_COLORINGCONTROLLER_H

#ifdef BALL_UI_V2

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

			public:
				explicit ColoringController(Representation* rep = nullptr,
				                            QObject* parent = nullptr);
				~ColoringController() override;

				Representation* representation() const { return rep_; }
				void setRepresentation(Representation* rep);

				int coloringMethod() const { return coloring_method_; }

			public Q_SLOTS:
				void apply();
				void revert();
				void setColoringMethod(int m);

			Q_SIGNALS:
				void coloringMethodChanged(int m);
				void appliedStub();

			private:
				Representation* rep_;
				int coloring_method_;
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_UI_V2
#endif // BALL_VIEW_KERNEL_CONTROLLERS_COLORINGCONTROLLER_H
