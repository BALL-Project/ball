// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 Plan 05 — BackgroundSection: Scene-tab section bound
// to StageController.backgroundColor. No new Controller — the Stage's
// background colour is owned by StageController so we just bind a
// second view onto the same property.
//

#ifndef BALL_VIEW_WIDGETS_INSPECTOR_SECTIONS_BACKGROUNDSECTION_H
#define BALL_VIEW_WIDGETS_INSPECTOR_SECTIONS_BACKGROUNDSECTION_H

#ifdef BALL_UI_V2

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#include <BALL/VIEW/WIDGETS/inspector/inspectorSection.h>

#include <QtCore/QTimer>
#include <QtGui/QColor>

namespace BALL
{
	namespace VIEW
	{
		class StageController;
		class SwatchButton;

		class BALL_VIEW_EXPORT BackgroundSection : public InspectorSection
		{
			Q_OBJECT

			public:
				explicit BackgroundSection(StageController* stage_controller,
				                           QWidget* parent = nullptr);
				~BackgroundSection() override;

			private Q_SLOTS:
				void onColorPicked_(const QColor& c);
				void onControllerColorChanged_(const QColor& c);
				void onDebounceFire_();

			private:
				StageController* controller_;
				QTimer           debounce_;
				SwatchButton*    swatch_;
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_UI_V2
#endif // BALL_VIEW_WIDGETS_INSPECTOR_SECTIONS_BACKGROUNDSECTION_H
