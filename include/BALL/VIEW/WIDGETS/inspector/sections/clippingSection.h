// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.51 Plan 01 — ClippingSection: Representation-tab section
// bound to ClippingController.
//

#ifndef BALL_VIEW_WIDGETS_INSPECTOR_SECTIONS_CLIPPINGSECTION_H
#define BALL_VIEW_WIDGETS_INSPECTOR_SECTIONS_CLIPPINGSECTION_H


#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#include <BALL/VIEW/WIDGETS/inspector/inspectorSection.h>

#include <QtCore/QTimer>

class QCheckBox;

namespace BALL
{
	namespace VIEW
	{
		class ClippingController;
		class LabeledSlider;

		class BALL_VIEW_EXPORT ClippingSection : public InspectorSection
		{
			Q_OBJECT

			public:
				explicit ClippingSection(ClippingController* controller,
				                         QWidget* parent = nullptr);
				~ClippingSection() override;

			private Q_SLOTS:
				void onEnabledChanged_(bool b);
				void onOffsetChanged_(int v);
				void onCappedChanged_(bool b);
				void onControllerEnabledChanged_(bool b);
				void onControllerOffsetChanged_(float v);
				void onControllerCappedChanged_(bool b);
				void onDebounceFire_();

			private:
				ClippingController* controller_;
				QTimer              debounce_;
				QCheckBox*          enabled_;
				LabeledSlider*      offset_;
				QCheckBox*          capped_;
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_WIDGETS_INSPECTOR_SECTIONS_CLIPPINGSECTION_H
