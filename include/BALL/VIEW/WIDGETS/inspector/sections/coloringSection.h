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

#include <QtCore/QTimer>

class QComboBox;
class QStackedWidget;

namespace BALL
{
	namespace VIEW
	{
		class ColoringController;
		class LabeledSlider;

		class BALL_VIEW_EXPORT ColoringSection : public InspectorSection
		{
			Q_OBJECT

			public:
				explicit ColoringSection(ColoringController* controller,
				                         QWidget* parent = nullptr);
				~ColoringSection() override;

			private Q_SLOTS:
				void onMethodChosen_(int idx);
				void onValueMinChanged_(int v);
				void onValueMaxChanged_(int v);
				void onControllerMethodChanged_(int m);
				void onControllerValueMinChanged_(float v);
				void onControllerValueMaxChanged_(float v);
				void onDebounceFire_();

			private:
				ColoringController* controller_;
				QTimer              debounce_;
				QComboBox*          method_;

				// v1.7.x-17 — per-method options in a QStackedWidget below
				// the Method dropdown; the visible page tracks the selected
				// coloring method. The one populated page is the value-range
				// (Min/Max) shared by the value-based coloring methods.
				QStackedWidget*     type_options_;
				LabeledSlider*      value_min_;
				LabeledSlider*      value_max_;
				int                 page_empty_;
				int                 page_value_range_;

				void scheduleApply_();
				void showPageForMethod_(int method);
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_WIDGETS_INSPECTOR_SECTIONS_COLORINGSECTION_H
