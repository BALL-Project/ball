// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 Plan 05 — StereoController (read-only mirror).
//
// Source dialog: source/VIEW/DIALOGS/stereoSettingsDialog.C (~159 LOC).
//

#ifndef BALL_VIEW_KERNEL_CONTROLLERS_STEREOCONTROLLER_H
#define BALL_VIEW_KERNEL_CONTROLLERS_STEREOCONTROLLER_H

#ifdef BALL_UI_V2

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#include <QtCore/QObject>

namespace BALL
{
	namespace VIEW
	{
		class Stage;

		/**
		 * Stereo-rendering mode + eye/focal distances. Read-only
		 * mirror; cut-over plan migrates the legacy
		 * StereoSettingsDialog::apply() into apply().
		 */
		class BALL_VIEW_EXPORT StereoController : public QObject
		{
			Q_OBJECT

			Q_PROPERTY(bool stereoEnabled READ stereoEnabled WRITE setStereoEnabled NOTIFY stereoEnabledChanged)
			Q_PROPERTY(float eyeDistance READ eyeDistance WRITE setEyeDistance NOTIFY eyeDistanceChanged)
			Q_PROPERTY(float focalDistance READ focalDistance WRITE setFocalDistance NOTIFY focalDistanceChanged)
			Q_PROPERTY(bool swapSideBySide READ swapSideBySide WRITE setSwapSideBySide NOTIFY swapSideBySideChanged)

			public:
				explicit StereoController(Stage* stage = nullptr,
				                          QObject* parent = nullptr);
				~StereoController() override;

				Stage* stage() const { return stage_; }
				void setStage(Stage* stage);

				bool  stereoEnabled()  const { return enabled_; }
				float eyeDistance()    const { return eye_distance_; }
				float focalDistance()  const { return focal_distance_; }
				bool  swapSideBySide() const { return swap_sbs_; }

			public Q_SLOTS:
				void apply();
				void revert();
				void setStereoEnabled(bool b);
				void setEyeDistance(float v);
				void setFocalDistance(float v);
				void setSwapSideBySide(bool b);

			Q_SIGNALS:
				void stereoEnabledChanged(bool b);
				void eyeDistanceChanged(float v);
				void focalDistanceChanged(float v);
				void swapSideBySideChanged(bool b);
				void appliedStub();

			private:
				Stage* stage_;
				bool   enabled_;
				float  eye_distance_;
				float  focal_distance_;
				bool   swap_sbs_;
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_UI_V2
#endif // BALL_VIEW_KERNEL_CONTROLLERS_STEREOCONTROLLER_H
