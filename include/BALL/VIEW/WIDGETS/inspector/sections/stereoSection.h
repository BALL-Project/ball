// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 Plan 05 — StereoSection: Scene-tab section bound to
// StereoController.
//

#ifndef BALL_VIEW_WIDGETS_INSPECTOR_SECTIONS_STEREOSECTION_H
#define BALL_VIEW_WIDGETS_INSPECTOR_SECTIONS_STEREOSECTION_H

#ifdef BALL_UI_V2

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
		class StereoController;
		class LabeledSlider;

		class BALL_VIEW_EXPORT StereoSection : public InspectorSection
		{
			Q_OBJECT

			public:
				explicit StereoSection(StereoController* controller,
				                       QWidget* parent = nullptr);
				~StereoSection() override;

			private Q_SLOTS:
				void onEnabledToggled_(bool b);
				void onEyeChanged_(int v);
				void onFocalChanged_(int v);
				void onSwapToggled_(bool b);
				void onControllerEnabledChanged_(bool b);
				void onControllerEyeChanged_(float v);
				void onControllerFocalChanged_(float v);
				void onControllerSwapChanged_(bool b);
				void onDebounceFire_();

			private:
				StereoController* controller_;
				QTimer            debounce_;
				QCheckBox*        enabled_box_;
				LabeledSlider*    eye_slider_;
				LabeledSlider*    focal_slider_;
				QCheckBox*        swap_box_;
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_UI_V2
#endif // BALL_VIEW_WIDGETS_INSPECTOR_SECTIONS_STEREOSECTION_H
