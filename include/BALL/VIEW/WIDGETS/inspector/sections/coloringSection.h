// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 Plan 04 — ColoringSection: Representation-tab section
// bound to ColoringController.
//

#ifndef BALL_VIEW_WIDGETS_INSPECTOR_SECTIONS_COLORINGSECTION_H
#define BALL_VIEW_WIDGETS_INSPECTOR_SECTIONS_COLORINGSECTION_H

#ifdef BALL_UI_V2

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#include <BALL/VIEW/WIDGETS/inspector/inspectorSection.h>

#include <QtCore/QTimer>

class QComboBox;

namespace BALL
{
	namespace VIEW
	{
		class ColoringController;

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
				void onDebounceFire_();

			private:
				ColoringController* controller_;
				QTimer              debounce_;
				QComboBox*          method_;
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_UI_V2
#endif // BALL_VIEW_WIDGETS_INSPECTOR_SECTIONS_COLORINGSECTION_H
