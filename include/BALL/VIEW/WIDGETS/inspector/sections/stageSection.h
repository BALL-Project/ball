// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 — StageSection: Scene-tab Inspector section that
// binds to StageController. Exemplar pattern for the 12 other
// sections planned across the 3 tabs.
//

#ifndef BALL_VIEW_WIDGETS_INSPECTOR_SECTIONS_STAGESECTION_H
#define BALL_VIEW_WIDGETS_INSPECTOR_SECTIONS_STAGESECTION_H

// Phase 999.44: gated by BALL_UI_V2 (see inspectorSection.h).


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
		class StageController;
		class SwatchButton;
		class LabeledSlider;

		/**
		 * Scene-tab Inspector section for Stage settings. Replaces
		 * the legacy StageSettings dialog (~720 LOC) for the controls
		 * exposed in the Inspector — background colour, mouse / wheel
		 * sensitivity stand-ins (mapped to fog intensity for the
		 * read-only exemplar), show-coordinate-system, fog-enabled.
		 *
		 * Bound to a StageController (this plan ships the read-only
		 * mirror); user edits debounce-commit at 100ms via a single
		 * QTimer::singleShot per logical change. On debounce fire the
		 * controller's apply() runs — currently a Log::info stub plus
		 * the appliedStub signal so the live-commit pipeline is
		 * end-to-end observable.
		 *
		 * INI state key: "Scene/Stage" (persisted by InspectorView).
		 */
		class BALL_VIEW_EXPORT StageSection : public InspectorSection
		{
			Q_OBJECT

			public:
				explicit StageSection(StageController* controller,
				                      QWidget* parent = nullptr);
				~StageSection() override;

			private Q_SLOTS:
				void onBackgroundColorChosen_(const QColor& c);
				void onCoordinateSystemToggled_(bool checked);
				void onFogIntensityChanged_(int v);
				void onControllerBackgroundColorChanged_(const QColor& c);
				void onControllerCoordinateSystemChanged_(bool b);
				void onControllerFogIntensityChanged_(float v);
				void onDebounceFire_();

			private:
				StageController* controller_;
				QTimer debounce_;

				SwatchButton*  bg_swatch_;
				QCheckBox*     show_coord_;
				LabeledSlider* fog_slider_;

				void scheduleApply_();
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_WIDGETS_INSPECTOR_SECTIONS_STAGESECTION_H
