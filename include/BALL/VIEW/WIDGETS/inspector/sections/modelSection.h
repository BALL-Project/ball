// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 Plan 04 — ModelSection: Representation-tab Inspector
// section bound to ModelController.
//

#ifndef BALL_VIEW_WIDGETS_INSPECTOR_SECTIONS_MODELSECTION_H
#define BALL_VIEW_WIDGETS_INSPECTOR_SECTIONS_MODELSECTION_H

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
		class ModelController;
		class LabeledSlider;

		class BALL_VIEW_EXPORT ModelSection : public InspectorSection
		{
			Q_OBJECT

			public:
				explicit ModelSection(ModelController* controller,
				                      QWidget* parent = nullptr);
				~ModelSection() override;

			private Q_SLOTS:
				void onModelTypeChosen_(int idx);
				void onDrawingModeChosen_(int idx);
				void onPrecisionChosen_(int idx);
				void onTransparencyChanged_(int v);
				void onControllerModelTypeChanged_(int t);
				void onControllerDrawingModeChanged_(int m);
				void onControllerPrecisionChanged_(int p);
				void onControllerTransparencyChanged_(int t);
				void onDebounceFire_();

			private:
				ModelController* controller_;
				QTimer           debounce_;
				QComboBox*       model_type_;
				QComboBox*       drawing_mode_;
				QComboBox*       precision_;
				LabeledSlider*   transparency_;

				void scheduleApply_();
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_UI_V2
#endif // BALL_VIEW_WIDGETS_INSPECTOR_SECTIONS_MODELSECTION_H
