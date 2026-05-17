// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 — BALLView Refresh: StageController exemplar.
//
// PATTERN — extract one *Controller per legacy dialog. Both
// surfaces (legacy dialog + new Inspector section) bind to the
// same controller. Consistency between the two during the
// coexistence period is guaranteed by construction, not by
// message-bus discipline. This is the primary risk mitigation
// from Handover §04-phase-inspector.md §Risks.
//
// The controller is QObject-only (no QtWidgets dependencies)
// so it is headless-testable.
//
// 8 more Controllers to extract — see
// source/VIEW/KERNEL/controllers/PATTERN.md for the full list +
// the source-dialog mapping.
//

#ifndef BALL_VIEW_KERNEL_CONTROLLERS_STAGECONTROLLER_H
#define BALL_VIEW_KERNEL_CONTROLLERS_STAGECONTROLLER_H

// Phase 999.44: gated by BALL_UI_V2 (see inspectorSection.h).


#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#include <QtCore/QObject>
#include <QtGui/QColor>

namespace BALL
{
	namespace VIEW
	{
		class Stage;
		class Scene;

		/**
		 * Presentation controller for the Stage subsystem.
		 *
		 * Owns the mirrored state for all Stage display settings:
		 * backgroundColor / showCoordinateSystem / fogIntensity / etc.
		 * Properties are exposed via Q_PROPERTY so both the legacy
		 * StageSettings dialog and the new Inspector StageSection bind
		 * with the same Qt connect() machinery.
		 *
		 * Initial extraction is a **read-only mirror** — apply() is
		 * stubbed (Log::warn) until a follow-up plan migrates the
		 * actual mutation logic from StageSettings::apply(). Both
		 * surfaces continue to call the legacy mutation path during
		 * the migration window so end users see no change in
		 * behaviour while the Inspector lands.
		 *
		 * Headless-testable: no QtWidgets dependencies. Construct
		 * against a Stage* and verify property reads in a unit test
		 * without spawning a QApplication.
		 */
		class BALL_VIEW_EXPORT StageController : public QObject
		{
			Q_OBJECT

			Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)
			Q_PROPERTY(bool showCoordinateSystem READ showCoordinateSystem WRITE setShowCoordinateSystem NOTIFY showCoordinateSystemChanged)
			Q_PROPERTY(float fogIntensity READ fogIntensity WRITE setFogIntensity NOTIFY fogIntensityChanged)
			Q_PROPERTY(float eyeDistance READ eyeDistance WRITE setEyeDistance NOTIFY eyeDistanceChanged)
			Q_PROPERTY(float focalDistance READ focalDistance WRITE setFocalDistance NOTIFY focalDistanceChanged)

			public:
				/**
				 * Construct against a Stage + Scene. Both pointers may be
				 * null at construction time and updated later via setStage()
				 * / setScene() — the controller mirrors state lazily.
				 */
				StageController(Stage* stage = nullptr,
				                Scene* scene = nullptr,
				                QObject* parent = nullptr);
				~StageController() override;

				Stage* stage() const { return stage_; }
				Scene* scene() const { return scene_; }
				void setStage(Stage* stage);
				void setScene(Scene* scene);

				QColor backgroundColor() const { return background_color_; }
				bool showCoordinateSystem() const { return show_coordinate_system_; }
				float fogIntensity() const { return fog_intensity_; }
				float eyeDistance() const { return eye_distance_; }
				float focalDistance() const { return focal_distance_; }

			public Q_SLOTS:
				/** Push the controller's mirrored state to the Stage/Scene. */
				void apply();

				/** Pull state from the Stage/Scene into the controller, emitting xxxChanged signals. */
				void revert();

				void setBackgroundColor(const QColor& c);
				void setShowCoordinateSystem(bool b);
				void setFogIntensity(float v);
				void setEyeDistance(float v);
				void setFocalDistance(float v);

			Q_SIGNALS:
				void backgroundColorChanged(const QColor& c);
				void showCoordinateSystemChanged(bool b);
				void fogIntensityChanged(float v);
				void eyeDistanceChanged(float v);
				void focalDistanceChanged(float v);

				/**
				 * Emitted by apply() during the migration window — the
				 * actual mutation logic still lives in StageSettings.
				 * Wired by InspectorView so the live-commit pipeline is
				 * end-to-end observable in the Log.
				 */
				void appliedStub();

			private:
				Stage* stage_;
				Scene* scene_;

				// Mirrored state — populated by revert(), pushed by apply().
				QColor background_color_;
				bool   show_coordinate_system_;
				float  fog_intensity_;
				float  eye_distance_;
				float  focal_distance_;
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_KERNEL_CONTROLLERS_STAGECONTROLLER_H
