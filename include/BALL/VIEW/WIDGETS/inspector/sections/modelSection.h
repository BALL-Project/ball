// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 Plan 04 — ModelSection: Representation-tab Inspector
// section bound to ModelController.
//

#ifndef BALL_VIEW_WIDGETS_INSPECTOR_SECTIONS_MODELSECTION_H
#define BALL_VIEW_WIDGETS_INSPECTOR_SECTIONS_MODELSECTION_H


#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#include <BALL/VIEW/WIDGETS/inspector/inspectorSection.h>

#include <QtCore/QTimer>

class QComboBox;
class QCheckBox;
class QStackedWidget;

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
				void onBallRadiusChanged_(int v);
				void onBallStickBondRadiusChanged_(int v);
				void onDashedBondsToggled_(bool on);
				void onStickRadiusChanged_(int v);
				void onSurfaceProbeRadiusChanged_(int v);
				void onCartoonTubeRadiusChanged_(int v);
				void onControllerModelTypeChanged_(int t);
				void onControllerDrawingModeChanged_(int m);
				void onControllerPrecisionChanged_(int p);
				void onControllerTransparencyChanged_(int t);
				void onControllerBallRadiusChanged_(float v);
				void onControllerBallStickBondRadiusChanged_(float v);
				void onControllerDashedBondsChanged_(bool v);
				void onControllerStickRadiusChanged_(float v);
				void onControllerSurfaceProbeRadiusChanged_(float v);
				void onControllerCartoonTubeRadiusChanged_(float v);
				void onDebounceFire_();

			private:
				ModelController* controller_;
				QTimer           debounce_;
				QComboBox*       model_type_;
				QComboBox*       drawing_mode_;
				QComboBox*       precision_;
				LabeledSlider*   transparency_;

				// v1.7.x-16 — type-specific controls in a QStackedWidget
				// below the generic controls; the visible page tracks the
				// selected model type.
				QStackedWidget*  type_options_;
				LabeledSlider*   bs_sphere_radius_;
				LabeledSlider*   bs_bond_radius_;
				QCheckBox*       bs_dashed_bonds_;
				LabeledSlider*   stick_radius_;
				LabeledSlider*   surface_probe_radius_;
				LabeledSlider*   cartoon_tube_radius_;
				int              page_empty_;
				int              page_ball_and_stick_;
				int              page_stick_;
				int              page_surface_;
				int              page_cartoon_;

				void scheduleApply_();
				void showPageForModelType_(int t);
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_WIDGETS_INSPECTOR_SECTIONS_MODELSECTION_H
