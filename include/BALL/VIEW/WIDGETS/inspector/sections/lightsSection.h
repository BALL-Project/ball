// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 Plan 05 — LightsSection: Scene-tab section bound to
// LightController.
//

#ifndef BALL_VIEW_WIDGETS_INSPECTOR_SECTIONS_LIGHTSSECTION_H
#define BALL_VIEW_WIDGETS_INSPECTOR_SECTIONS_LIGHTSSECTION_H


#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#include <BALL/VIEW/WIDGETS/inspector/inspectorSection.h>

#include <QtCore/QTimer>

class QLabel;

namespace BALL
{
	namespace VIEW
	{
		class LightController;
		class LabeledSlider;

		class BALL_VIEW_EXPORT LightsSection : public InspectorSection
		{
			Q_OBJECT

			public:
				explicit LightsSection(LightController* controller,
				                       QWidget* parent = nullptr);
				~LightsSection() override;

			private Q_SLOTS:
				void onAmbientChanged_(int v);
				void onControllerLightCountChanged_(int c);
				void onControllerAmbientChanged_(float v);
				void onDebounceFire_();

			private:
				LightController* controller_;
				QTimer           debounce_;
				QLabel*          count_label_;
				LabeledSlider*   ambient_slider_;
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_WIDGETS_INSPECTOR_SECTIONS_LIGHTSSECTION_H
