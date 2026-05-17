// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 Plan 04 — ClippingController (read-only mirror).
//
// Source dialogs: source/VIEW/DIALOGS/clippingDialog.C (~119 LOC) +
// setClippingPlane.C (~92 LOC); merged per Handover §architecture.
//

#ifndef BALL_VIEW_KERNEL_CONTROLLERS_CLIPPINGCONTROLLER_H
#define BALL_VIEW_KERNEL_CONTROLLERS_CLIPPINGCONTROLLER_H

#ifdef BALL_UI_V2

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#include <QtCore/QObject>

namespace BALL
{
	namespace VIEW
	{
		class Scene;

		/**
		 * Clipping-plane controller — enabled state + plane offset.
		 * Read-only mirror; the legacy ClippingDialog +
		 * SetClippingPlane dialogs still own mutation until the
		 * cut-over plan.
		 */
		class BALL_VIEW_EXPORT ClippingController : public QObject
		{
			Q_OBJECT

			Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged)
			Q_PROPERTY(float offset READ offset WRITE setOffset NOTIFY offsetChanged)
			Q_PROPERTY(bool capped READ capped WRITE setCapped NOTIFY cappedChanged)

			public:
				explicit ClippingController(Scene* scene = nullptr,
				                            QObject* parent = nullptr);
				~ClippingController() override;

				Scene* scene() const { return scene_; }
				void setScene(Scene* scene);

				bool  enabled() const { return enabled_; }
				float offset() const  { return offset_; }
				bool  capped() const  { return capped_; }

			public Q_SLOTS:
				void apply();
				void revert();
				void setEnabled(bool b);
				void setOffset(float v);
				void setCapped(bool b);

			Q_SIGNALS:
				void enabledChanged(bool b);
				void offsetChanged(float v);
				void cappedChanged(bool b);
				void appliedStub();

			private:
				Scene* scene_;
				bool   enabled_;
				float  offset_;
				bool   capped_;
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_UI_V2
#endif // BALL_VIEW_KERNEL_CONTROLLERS_CLIPPINGCONTROLLER_H
