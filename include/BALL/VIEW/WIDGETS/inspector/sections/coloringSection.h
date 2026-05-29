// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 Plan 04 — ColoringSection: Representation-tab section
// bound to ColoringController.
//

#ifndef BALL_VIEW_WIDGETS_INSPECTOR_SECTIONS_COLORINGSECTION_H
#define BALL_VIEW_WIDGETS_INSPECTOR_SECTIONS_COLORINGSECTION_H


#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#include <BALL/VIEW/WIDGETS/inspector/inspectorSection.h>

class QComboBox;
class QStackedWidget;

namespace BALL
{
	namespace VIEW
	{
		class ColoringController;
		class ValueRangeWidget;

		class BALL_VIEW_EXPORT ColoringSection : public InspectorSection
		{
			Q_OBJECT

			public:
				explicit ColoringSection(ColoringController* controller,
				                         QWidget* parent = nullptr);
				~ColoringSection() override;

			private Q_SLOTS:
				void onMethodChosen_(int idx);
				void onControllerMethodChanged_(int m);
				void onControllerValueMinChanged_(float v);
				void onControllerValueMaxChanged_(float v);

				// 999.63 — value-range histogram widget signals.
				void onRangeChanged_(float min, float max);     // drag → preview.
				void onRangeCommitted_(float min, float max);   // release → apply.
				void onAutoFit_();
				void onFullRange_();
				void onRobustRange_();
				void onWidgetReset_();

			private:
				ColoringController* controller_;
				QComboBox*          method_;

				// v1.7.x-17 → 999.63 — per-method options in a QStackedWidget
				// below the Method dropdown; the visible page tracks the selected
				// coloring method. The populated page now hosts the value-range
				// histogram widget (replacing the v1.7.x-17 Min/Max sliders),
				// shared by the value-based coloring methods.
				QStackedWidget*     type_options_;
				ValueRangeWidget*   range_widget_;
				int                 page_empty_;
				int                 page_value_range_;

				void showPageForMethod_(int method);
				/// Pull distribution() from the controller into the widget for
				/// the current method (no-op for non-value methods).
				void refreshDistribution_();
				/// Method-defined full-range limits (Full range preset).
				void fullRangeLimitsForMethod_(int method, float& lo, float& hi) const;
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_WIDGETS_INSPECTOR_SECTIONS_COLORINGSECTION_H
