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

#ifndef BALL_VIEW_KERNEL_CONTROLLERS_CONTROLLER_H
# include <BALL/VIEW/KERNEL/controllers/controller.h>
#endif

#ifndef BALL_VIEW_KERNEL_CONTROLLERS_APPLYPAYLOAD_H
# include <BALL/VIEW/KERNEL/controllers/applyPayload.h>
#endif

#include <QtCore/QObject>
#include <QtGui/QColor>

// Phase 999.58-02 — RenderSetup::RendererType is the renderer-switch
// field the extended apply() now owns (OPENGL_RENDERER / RTFACT_RENDERER);
// Camera::ProjectionMode is the projection field (perspective/orthographic).
#ifndef BALL_VIEW_RENDERING_RENDERSETUP_H
# include <BALL/VIEW/RENDERING/renderSetup.h>
#endif
#ifndef BALL_VIEW_RENDERING_CAMERA_H
# include <BALL/VIEW/RENDERING/camera.h>
#endif

// ---------------------------------------------------------------------------
// 999.59-03 per-controller contract-fixture activation symbol (Codex MEDIUM
// #7) — flips stage_contract_test's parity block live (see
// test/contract/contractFixtureActivation.h). Defined ONLY because apply()
// below returns bool. StageController is the one scene-domain controller whose
// §2a declared invalidation is NOT a soft refresh: it carries the documented
// scene-structural side effects (rep-delete on vertex-buffer-mode toggle +
// renderer switch, then mc->redrawAllRepresentations()).
// ---------------------------------------------------------------------------
#define BALL_VIEW_STAGE_APPLY_BOOL 1

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
		class BALL_VIEW_EXPORT StageController : public Controller
		{
			Q_OBJECT

			Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)
			Q_PROPERTY(bool showCoordinateSystem READ showCoordinateSystem WRITE setShowCoordinateSystem NOTIFY showCoordinateSystemChanged)
			Q_PROPERTY(float fogIntensity READ fogIntensity WRITE setFogIntensity NOTIFY fogIntensityChanged)
			Q_PROPERTY(float eyeDistance READ eyeDistance WRITE setEyeDistance NOTIFY eyeDistanceChanged)
			Q_PROPERTY(float focalDistance READ focalDistance WRITE setFocalDistance NOTIFY focalDistanceChanged)

			// Phase 999.58-02 — the remaining non-stereo render-config field
			// set the legacy StageSettings::apply() owned (stageSettings.C:233-309).
			Q_PROPERTY(bool perspectiveProjection READ perspectiveProjection WRITE setPerspectiveProjection NOTIFY perspectiveProjectionChanged)
			Q_PROPERTY(bool showLightSources READ showLightSources WRITE setShowLightSources NOTIFY showLightSourcesChanged)
			Q_PROPERTY(float animationSmoothness READ animationSmoothness WRITE setAnimationSmoothness NOTIFY animationSmoothnessChanged)
			Q_PROPERTY(bool offScreenRendering READ offScreenRendering WRITE setOffScreenRendering NOTIFY offScreenRenderingChanged)
			Q_PROPERTY(int offScreenFactor READ offScreenFactor WRITE setOffScreenFactor NOTIFY offScreenFactorChanged)
			Q_PROPERTY(QColor cappingColor READ cappingColor WRITE setCappingColor NOTIFY cappingColorChanged)
			Q_PROPERTY(bool fpsEnabled READ fpsEnabled WRITE setFPSEnabled NOTIFY fpsEnabledChanged)
			Q_PROPERTY(bool preview READ preview WRITE setPreview NOTIFY previewChanged)
			Q_PROPERTY(bool vertexBuffersEnabled READ vertexBuffersEnabled WRITE setVertexBuffersEnabled NOTIFY vertexBuffersEnabledChanged)
			Q_PROPERTY(bool smoothLines READ smoothLines WRITE setSmoothLines NOTIFY smoothLinesChanged)
			Q_PROPERTY(float downsamplingFactor READ downsamplingFactor WRITE setDownsamplingFactor NOTIFY downsamplingFactorChanged)
			Q_PROPERTY(int rendererType READ rendererType WRITE setRendererType NOTIFY rendererTypeChanged)
			// Mouse / wheel sensitivity have no backend readback (the
			// InteractionModeManager exposes setters only), so they are
			// write-only mirrors driven by the Inspector; apply() pushes them
			// only when explicitly set (sentinel < 0 means "leave live value").
			Q_PROPERTY(float mouseSensitivity READ mouseSensitivity WRITE setMouseSensitivity NOTIFY mouseSensitivityChanged)
			Q_PROPERTY(float mouseWheelSensitivity READ mouseWheelSensitivity WRITE setMouseWheelSensitivity NOTIFY mouseWheelSensitivityChanged)

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

				// Phase 999.58-02 — extended non-stereo render-config getters.
				bool perspectiveProjection() const { return perspective_projection_; }
				bool showLightSources() const { return show_light_sources_; }
				float animationSmoothness() const { return animation_smoothness_; }
				bool offScreenRendering() const { return offscreen_rendering_; }
				int offScreenFactor() const { return offscreen_factor_; }
				QColor cappingColor() const { return capping_color_; }
				bool fpsEnabled() const { return fps_enabled_; }
				bool preview() const { return preview_; }
				bool vertexBuffersEnabled() const { return vertex_buffers_enabled_; }
				bool smoothLines() const { return smooth_lines_; }
				float downsamplingFactor() const { return downsampling_factor_; }
				int rendererType() const { return renderer_type_; }
				float mouseSensitivity() const { return mouse_sensitivity_; }
				float mouseWheelSensitivity() const { return mouse_wheel_sensitivity_; }

				/// v1.7.x-24 — true while apply() is mutating the Stage.
				/// Notification slots that could re-trigger apply() must
				/// early-return on this to break the re-entrancy cascade.
				/// 999.59-03 — re-entrancy state now lives in the base depth
				/// counter (applying_depth_).
				bool isApplying() const { return applying_depth_ > 0; }

				/**
				 * Push the controller's mirrored state to the Stage/Scene,
				 * command-shaped per §2. Returns true when the Stage/Scene was
				 * mutated, false when dropped (re-entry) or rejected (no Stage /
				 * MainControl busy). Overrides Controller::apply(). Not a
				 * string-based Qt slot — callers invoke controller_->apply()
				 * directly, so the bool return is binding-surface-compatible
				 * with the prior void apply().
				 * @return true if the owner was mutated, false otherwise.
				 */
				bool apply() override;

				/**
				 * Reset the mirror to the owner's current state (999.64 reset
				 * path). Overrides Controller::reset(); delegates to revert().
				 */
				void reset() override;

			public Q_SLOTS:
				/** Pull state from the Stage/Scene into the controller, emitting xxxChanged signals. */
				void revert();

				void setBackgroundColor(const QColor& c);
				void setShowCoordinateSystem(bool b);
				void setFogIntensity(float v);
				void setEyeDistance(float v);
				void setFocalDistance(float v);

				// Phase 999.58-02 — extended non-stereo render-config setters.
				void setPerspectiveProjection(bool b);
				void setShowLightSources(bool b);
				void setAnimationSmoothness(float v);
				void setOffScreenRendering(bool b);
				void setOffScreenFactor(int factor);
				void setCappingColor(const QColor& c);
				void setFPSEnabled(bool b);
				void setPreview(bool b);
				void setVertexBuffersEnabled(bool b);
				void setSmoothLines(bool b);
				void setDownsamplingFactor(float v);
				void setRendererType(int type);
				void setMouseSensitivity(float v);
				void setMouseWheelSensitivity(float v);

			Q_SIGNALS:
				void backgroundColorChanged(const QColor& c);
				void showCoordinateSystemChanged(bool b);
				void fogIntensityChanged(float v);
				void eyeDistanceChanged(float v);
				void focalDistanceChanged(float v);

				// Phase 999.58-02 — extended non-stereo render-config signals.
				void perspectiveProjectionChanged(bool b);
				void showLightSourcesChanged(bool b);
				void animationSmoothnessChanged(float v);
				void offScreenRenderingChanged(bool b);
				void offScreenFactorChanged(int factor);
				void cappingColorChanged(const QColor& c);
				void fpsEnabledChanged(bool b);
				void previewChanged(bool b);
				void vertexBuffersEnabledChanged(bool b);
				void smoothLinesChanged(bool b);
				void downsamplingFactorChanged(float v);
				void rendererTypeChanged(int type);
				void mouseSensitivityChanged(float v);
				void mouseWheelSensitivityChanged(float v);

				/**
				 * Emitted by apply() during the migration window — the
				 * actual mutation logic still lives in StageSettings.
				 * Wired by InspectorView so the live-commit pipeline is
				 * end-to-end observable in the Log.
				 */
				void appliedStub();

			protected:
				/**
				 * §2a declared invalidation — StageController's documented
				 * scene-structural side effects, NOT a soft refresh and NOT a
				 * blanket rebuild. On a vertex-buffer-mode toggle or renderer
				 * switch the affected representations are deleted/rebuilt and
				 * mc->redrawAllRepresentations() is called; the background/fog
				 * render state is refreshed via Scene::refreshSceneRenderState().
				 * Kept in this ONE place per §2a. Overrides
				 * Controller::invalidateDeclared_().
				 */
				void invalidateDeclared_() override;

			private:
				/**
				 * §13-cookbook mutation body, moved out of apply(): pushes the
				 * mirrored field set onto the Stage / Scene / renderer (the full
				 * non-stereo render config the legacy StageSettings::apply()
				 * owned), INCLUDING the rep-delete-on-vertex-buffer-toggle side
				 * effect. Precondition stage_ != nullptr guaranteed by apply().
				 * Does NOT itself drive the final refresh (that is
				 * invalidateDeclared_()). @return true (always installs state).
				 */
				bool applyInternal_();

				/// Capture-only reversible intent (§2 step 7; v2.0 UndoStack).
				void recordIntent_(const ApplyPayload& payload);

				Stage* stage_;
				Scene* scene_;

				// Mirrored state — populated by revert(), pushed by apply().
				QColor background_color_;
				bool   show_coordinate_system_;
				float  fog_intensity_;
				float  eye_distance_;
				float  focal_distance_;

				// Phase 999.58-02 — the rest of the non-stereo render config
				// the legacy StageSettings::apply() owned.
				bool   perspective_projection_;
				bool   show_light_sources_;
				float  animation_smoothness_;
				bool   offscreen_rendering_;
				int    offscreen_factor_;
				QColor capping_color_;
				bool   fps_enabled_;
				bool   preview_;
				bool   vertex_buffers_enabled_;
				bool   smooth_lines_;
				float  downsampling_factor_;
				int    renderer_type_;          // RenderSetup::RendererType as int
				// Mouse / wheel sensitivity have no backend readback; a sentinel
				// < 0 means "unset — leave the live InteractionModeManager value
				// untouched in apply()".
				float  mouse_sensitivity_;
				float  mouse_wheel_sensitivity_;

				// 999.59-03 — re-entrancy state now lives in the base depth
				// counter (applying_depth_); see ControllerApplyGuard.

				ApplyPayload last_payload_;  // 999.59-03 — capture-only reversible intent.
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_KERNEL_CONTROLLERS_STAGECONTROLLER_H
