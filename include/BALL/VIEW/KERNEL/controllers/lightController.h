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

#ifndef BALL_VIEW_KERNEL_STAGE_H
# include <BALL/VIEW/KERNEL/stage.h>
#endif

#include <QtCore/QObject>

#include <vector>

namespace BALL
{
	namespace VIEW
	{
		class Stage;

		/**
		 * Lighting controller. Owns the COMPLETE per-light list (the
		 * authoritative lights_ vector the legacy LightSettings dialog
		 * held) plus the aggregate ambient-intensity slider value.
		 *
		 * apply() mirrors LightSettings::apply()
		 * (source/VIEW/DIALOGS/lightSettings.C:384-393): clear the Stage
		 * light sources and re-add the controller's authoritative list,
		 * then Scene::lightsUpdated(true). An AMBIENT default is injected
		 * for an empty list so existing rendered scenes are unchanged
		 * (mirrors defaultsPressed()).
		 *
		 * revert() / syncFromStage() pull the full live light list from
		 * the Stage (mirrors LightSettings::updateFromStage,
		 * lightSettings.C:69-86). syncFromStage() is the public entry
		 * point the Scene render-refresh / camera-move call sites use in
		 * place of the legacy light_settings_->updateFromStage().
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

				/// The controller's authoritative full light list (the
				/// vector LightSettings::lights_ used to hold). apply()
				/// pushes this to the Stage; syncFromStage() refills it.
				const std::vector<LightSource>& getLightSources() const { return lights_; }
				void setLightSources(const std::vector<LightSource>& lights);

				/// v1.7.x-24 — true while apply() is mutating the Stage.
				/// Notification slots that could re-trigger apply() must
				/// early-return on this to break the re-entrancy cascade.
				bool isApplying() const { return applying_; }

			public Q_SLOTS:
				void apply();
				void revert();

				/// Public stage-resync entry point. Re-reads the full live
				/// light list from the Stage into the controller (mirrors
				/// LightSettings::updateFromStage). The Scene camera-move /
				/// render-refresh call sites call this in place of the
				/// legacy light_settings_->updateFromStage() (999.58-01).
				/// Implemented as a thin alias of revert().
				void syncFromStage();

				void setAmbientIntensity(float v);

			Q_SIGNALS:
				void lightCountChanged(int c);
				void ambientIntensityChanged(float v);
				void appliedStub();

			private:
				Stage* stage_;
				int    light_count_;
				float  ambient_intensity_;

				// Authoritative full per-light list (position / direction /
				// type / intensity / color / attenuation / relative-to-camera)
				// — the same role LightSettings::lights_ played. apply() clears
				// the Stage and re-adds this list.
				std::vector<LightSource> lights_;

				// v1.7.x-24 — re-entrancy shield (see ControllerApplyGuard).
				bool   applying_;
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_KERNEL_CONTROLLERS_LIGHTCONTROLLER_H
