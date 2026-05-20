// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 Plan 05 — LightController (read-only mirror).
//
// Source dialog: source/VIEW/DIALOGS/lightSettings.C (~538 LOC).
//

#ifndef BALL_VIEW_KERNEL_CONTROLLERS_LIGHTCONTROLLER_H
#define BALL_VIEW_KERNEL_CONTROLLERS_LIGHTCONTROLLER_H


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
		 * Aggregate lighting controls: light count + ambient
		 * intensity. Mirrors the legacy LightSettings dialog;
		 * cut-over plan migrates the per-light editor.
		 */
		class BALL_VIEW_EXPORT LightController : public QObject
		{
			Q_OBJECT

			Q_PROPERTY(int lightCount READ lightCount NOTIFY lightCountChanged)
			Q_PROPERTY(float ambientIntensity READ ambientIntensity WRITE setAmbientIntensity NOTIFY ambientIntensityChanged)

			public:
				explicit LightController(Stage* stage = nullptr,
				                         QObject* parent = nullptr);
				~LightController() override;

				Stage* stage() const { return stage_; }
				void setStage(Stage* stage);

				int   lightCount()       const { return light_count_; }
				float ambientIntensity() const { return ambient_intensity_; }

				/// v1.7.x-24 — true while apply() is mutating the Stage.
				/// Notification slots that could re-trigger apply() must
				/// early-return on this to break the re-entrancy cascade.
				bool isApplying() const { return applying_; }

			public Q_SLOTS:
				void apply();
				void revert();
				void setAmbientIntensity(float v);

			Q_SIGNALS:
				void lightCountChanged(int c);
				void ambientIntensityChanged(float v);
				void appliedStub();

			private:
				Stage* stage_;
				int    light_count_;
				float  ambient_intensity_;

				// v1.7.x-24 — re-entrancy shield (see ControllerApplyGuard).
				bool   applying_;
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_KERNEL_CONTROLLERS_LIGHTCONTROLLER_H
