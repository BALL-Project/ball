// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 Plan 05 — CameraSection: Scene-tab section bound to
// CameraController. Shows the live view-point / look-at as read-only
// QLabels (cut-over plan turns them into editable QLineEdits).
//

#ifndef BALL_VIEW_WIDGETS_INSPECTOR_SECTIONS_CAMERASECTION_H
#define BALL_VIEW_WIDGETS_INSPECTOR_SECTIONS_CAMERASECTION_H

#ifdef BALL_UI_V2

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#include <BALL/VIEW/WIDGETS/inspector/inspectorSection.h>

class QLabel;
class QVector3D;

namespace BALL
{
	namespace VIEW
	{
		class CameraController;

		class BALL_VIEW_EXPORT CameraSection : public InspectorSection
		{
			Q_OBJECT

			public:
				explicit CameraSection(CameraController* controller,
				                       QWidget* parent = nullptr);
				~CameraSection() override;

			private Q_SLOTS:
				void onControllerPositionChanged_(const QVector3D& p);
				void onControllerLookAtChanged_(const QVector3D& p);

			private:
				CameraController* controller_;
				QLabel* position_label_;
				QLabel* lookat_label_;
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_UI_V2
#endif // BALL_VIEW_WIDGETS_INSPECTOR_SECTIONS_CAMERASECTION_H
