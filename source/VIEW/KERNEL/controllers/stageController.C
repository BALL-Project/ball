// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 — StageController implementation.
//

#include <BALL/VIEW/KERNEL/controllers/stageController.h>


#include <BALL/VIEW/KERNEL/stage.h>
#include <BALL/VIEW/KERNEL/stageMutation.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/representationManager.h>
#include <BALL/VIEW/KERNEL/representation.h>
#include <BALL/VIEW/KERNEL/clippingPlane.h>
#include <BALL/VIEW/KERNEL/MODES/interactionModeManager.h>
#include <BALL/VIEW/WIDGETS/scene.h>
#include <BALL/VIEW/RENDERING/RENDERERS/glRenderer.h>
#include <BALL/VIEW/RENDERING/camera.h>
#include <BALL/VIEW/DATATYPE/colorRGBA.h>
#include <BALL/VIEW/KERNEL/controllers/controllerApplyGuard.h>
#include <BALL/COMMON/logStream.h>
#include <BALL/CONCEPT/timeStamp.h>

// std::clamp is used by colorRGBAToQColor_ below.
#include <algorithm>
#include <sstream>

namespace BALL
{
	namespace VIEW
	{

		namespace
		{
			QColor colorRGBAToQColor_(const ColorRGBA& c)
			{
				// ColorRGBA channels are ColorUnits (0..1); QColor wants 0..255.
				// Explicit float casts disambiguate ColorUnit's many built-in
				// conversion operators (operator char/int/float/...).
				float r = static_cast<float>(c.getRed());
				float g = static_cast<float>(c.getGreen());
				float b = static_cast<float>(c.getBlue());
				float a = static_cast<float>(c.getAlpha());
				int ri = static_cast<int>(r * 255.0f);
				int gi = static_cast<int>(g * 255.0f);
				int bi = static_cast<int>(b * 255.0f);
				int ai = static_cast<int>(a * 255.0f);
				return QColor(std::clamp(ri, 0, 255),
				              std::clamp(gi, 0, 255),
				              std::clamp(bi, 0, 255),
				              std::clamp(ai, 0, 255));
			}

			// Compact owner-state snapshot for the ApplyPayload before/after
			// blobs (§2 step 3). Mirrors the harness OwnerSnapshot stage fields
			// (bg / fog / eye / focal).
			String snapshotStageState_(Stage* stage)
			{
				if (stage == nullptr) return String("stage{null}");
				std::ostringstream s;
				const ColorRGBA& bg = stage->getBackgroundColor();
				s << "bg=" << static_cast<float>(bg.getRed())
				  << ',' << static_cast<float>(bg.getGreen())
				  << ',' << static_cast<float>(bg.getBlue())
				  << ";fog=" << stage->getFogIntensity()
				  << ";eye=" << stage->getEyeDistance()
				  << ";focal=" << stage->getFocalDistance();
				return String(s.str());
			}
		}

		StageController::StageController(Stage* stage, Scene* scene, QObject* parent)
			: Controller(parent),
				stage_(stage),
				scene_(scene),
				background_color_(Qt::black),
				show_coordinate_system_(false),
				fog_intensity_(0.0f),
				eye_distance_(0.0f),
				focal_distance_(0.0f),
				// Phase 999.58-02 — extended non-stereo render config defaults.
				perspective_projection_(true),
				show_light_sources_(false),
				animation_smoothness_(0.0f),
				offscreen_rendering_(false),
				offscreen_factor_(1),
				capping_color_(Qt::black),
				fps_enabled_(false),
				preview_(false),
				vertex_buffers_enabled_(false),
				smooth_lines_(false),
				downsampling_factor_(1.0f),
				renderer_type_(static_cast<int>(RenderSetup::OPENGL_RENDERER)),
				mouse_sensitivity_(-1.0f),        // sentinel: unset
				mouse_wheel_sensitivity_(-1.0f)   // sentinel: unset
		{
			revert();
		}

		StageController::~StageController() = default;

		void StageController::setStage(Stage* stage)
		{
			if (stage_ == stage) return;
			stage_ = stage;
			revert();
		}

		void StageController::setScene(Scene* scene)
		{
			scene_ = scene;
		}

		void StageController::revert()
		{
			if (!stage_)
				return;
			QColor bg = colorRGBAToQColor_(stage_->getBackgroundColor());
			if (bg != background_color_)
			{
				background_color_ = bg;
				Q_EMIT backgroundColorChanged(background_color_);
			}
			bool cs = stage_->coordinateSystemEnabled();
			if (cs != show_coordinate_system_)
			{
				show_coordinate_system_ = cs;
				Q_EMIT showCoordinateSystemChanged(cs);
			}
			float fi = stage_->getFogIntensity();
			if (fi != fog_intensity_)
			{
				fog_intensity_ = fi;
				Q_EMIT fogIntensityChanged(fi);
			}
			float ed = stage_->getEyeDistance();
			if (ed != eye_distance_)
			{
				eye_distance_ = ed;
				Q_EMIT eyeDistanceChanged(ed);
			}
			float fd = stage_->getFocalDistance();
			if (fd != focal_distance_)
			{
				focal_distance_ = fd;
				Q_EMIT focalDistanceChanged(fd);
			}

			// Phase 999.58-02 — pull the extended non-stereo render config from
			// its live backend (Stage camera, Scene statics/members, GLRenderer,
			// ClippingPlane). Mouse / wheel sensitivity have no backend getter
			// (InteractionModeManager exposes setters only), so they stay at the
			// "unset" sentinel and are not read back here.
			bool persp = (stage_->getCamera().getProjectionMode() == Camera::PERSPECTIVE);
			if (persp != perspective_projection_)
			{
				perspective_projection_ = persp;
				Q_EMIT perspectiveProjectionChanged(persp);
			}

			bool sls = Scene::showLightSourcesEnabled();
			if (sls != show_light_sources_)
			{
				show_light_sources_ = sls;
				Q_EMIT showLightSourcesChanged(sls);
			}

			float as = Scene::getAnimationSmoothness();
			if (as != animation_smoothness_)
			{
				animation_smoothness_ = as;
				Q_EMIT animationSmoothnessChanged(as);
			}

			QColor cap = colorRGBAToQColor_(ClippingPlane::getCappingColor());
			if (cap != capping_color_)
			{
				capping_color_ = cap;
				Q_EMIT cappingColorChanged(cap);
			}

			if (scene_ != nullptr)
			{
				bool prev = scene_->usePreview();
				if (prev != preview_)
				{
					preview_ = prev;
					Q_EMIT previewChanged(prev);
				}

				float ds = scene_->getDownsamplingFactor();
				if (ds != downsampling_factor_)
				{
					downsampling_factor_ = ds;
					Q_EMIT downsamplingFactorChanged(ds);
				}

				GLRenderer& renderer = scene_->getGLRenderer();
				bool vb = renderer.vertexBuffersEnabled();
				if (vb != vertex_buffers_enabled_)
				{
					vertex_buffers_enabled_ = vb;
					Q_EMIT vertexBuffersEnabledChanged(vb);
				}

				bool sl = renderer.getSmoothLines();
				if (sl != smooth_lines_)
				{
					smooth_lines_ = sl;
					Q_EMIT smoothLinesChanged(sl);
				}
			}
		}

		bool StageController::apply()
		{
			// Phase 999.59-03 — cut over to the §2 `bool apply()` command
			// contract (ARCHITECTURE-CONTRACT.md §2 / §2a / §13). This
			// controller owns the full non-stereo render-config field set the
			// legacy StageSettings::apply() (stageSettings.C:233-309) wrote:
			// background + coordinate-system + fog + eye/focal (since 999.44),
			// plus mouse/wheel sensitivity, projection mode, show-light-sources,
			// animation smoothness, offscreen rendering, capping color,
			// FPS-enabled, preview, vertex buffers (incl. the rep-delete-on-
			// toggle side effect, now §2a-declared in invalidateDeclared_()),
			// smooth lines, downsampling, and the OpenGL/RTfact renderer switch.
			// swap-side-by-side is deliberately NOT written here — StereoController
			// owns it (avoids a double-write race; see stereoController.C).

			// ── 1. Preconditions (§2 step 1) ─────────────────────────────────
			if (stage_ == nullptr)
			{
				Log.warn() << "[StageController::apply] no Stage attached — skipping." << std::endl;
				return false;                    // rejected — nothing to mutate
			}

			// Phase 999.58-02 — defer while MainControl is busy (mirrors
			// StereoController::apply()). The extended apply() can trigger a
			// renderer switch and a bulk representation rebuild (vertex-buffer
			// toggle), so it must not run mid-busy-cycle (T-99858-04).
			MainControl* mc = MainControl::getInstance(0);
			if (mc != nullptr && mc->isBusy())
			{
				Log.info() << "[StageController::apply] MainControl busy — deferring." << std::endl;
				return false;                    // rejected — busy
			}

			// ── 2. Re-entrancy shield (§2 step 2 / §4) — nest-aware drop ─────
			// If a notification triggered by this apply() (e.g. the scene
			// refresh in invalidateDeclared_()) synchronously re-enters apply(),
			// drop rather than re-running the mutation — this is the cascade
			// class behind the v1.7.x-13 freeze. The scene-structural side
			// effects in invalidateDeclared_() run INSIDE this guard (T-99858-04).
			if (applying_depth_ > 0) return false;          // dropped — re-entry
			ControllerApplyGuard apply_guard(applying_depth_);

			// ── 3. Capture reversible intent (§2 step 3) ─────────────────────
			ApplyPayload payload;
			payload.command_id = "stage.apply";
			payload.before     = snapshotStageState_(stage_);
			payload.target     = stage_;
			payload.t_us       = PreciseTime::now().getMicroSeconds();

			// ── 4. Mutate the single owner (§2 step 4) ───────────────────────
			applyInternal_();

			payload.after = snapshotStageState_(stage_);

			// ── 5. Emit ONE typed event (§2 step 5) ──────────────────────────
			Q_EMIT appliedStub();

			// ── 6. Request the DECLARED invalidation (§2 step 6 / §2a) ───────
			// StageController declares scene-structural side effects (NOT a soft
			// refresh): the vertex-buffer rep-delete + renderer switch +
			// redrawAllRepresentations, plus the render-state refresh.
			invalidateDeclared_();

			// ── 7. Record reversible intent (§2 step 7) — capture only ───────
			recordIntent_(payload);

			return true;                          // mutated
		}

		bool StageController::applyInternal_()
		{
			// §13 cookbook step 2 — the field-push mutation body, moved out of
			// apply(). Precondition (stage_ != nullptr) guaranteed by apply().
			// Each call routes through the SAME backend the legacy
			// StageSettings::apply() used (stageSettings.C:238-308). The
			// scene-structural side effects (vertex-buffer rep-delete, renderer
			// switch, render-state refresh) are NOT here — they are the §2a
			// declared invalidation (invalidateDeclared_()).

			// background_color_ is QColor; Stage stores ColorRGBA.
			// ColorRGBA::set(const QColor&) handles the 0..255 →
			// 0..1 (ColorUnit) conversion + alpha.
			ColorRGBA c;
			c.set(background_color_);
			// §3b — Stage mutation flows through the single StageMutation friend.
			StageMutation(*stage_)
				.backgroundColor(c)
				.fogIntensity(fog_intensity_)
				.eyeDistance(eye_distance_)
				.focalDistance(focal_distance_);

			stage_->showCoordinateSystem(show_coordinate_system_);

			// Mouse / wheel sensitivity: push only when explicitly set
			// (sentinel < 0 means "unset" — leave the live value untouched so
			// the controller-driven applyPreferences path does not clobber the
			// user's interaction sensitivity with a default).
			Scene* sensitivity_scene = (scene_ != nullptr) ? scene_ : Scene::getInstance(0);
			if (sensitivity_scene != nullptr)
			{
				if (mouse_sensitivity_ >= 0.0f)
					sensitivity_scene->getInteractionModeManager().setMouseSensitivity(mouse_sensitivity_);
				if (mouse_wheel_sensitivity_ >= 0.0f)
					sensitivity_scene->getInteractionModeManager().setMouseWheelSensitivity(mouse_wheel_sensitivity_);
			}

			// Projection mode (perspective / orthographic).
			stage_->getCamera().setProjectionMode(perspective_projection_ ? Camera::PERSPECTIVE
			                                                               : Camera::ORTHOGRAPHIC);

			// Show light sources + animation smoothness (Scene statics).
			Scene::setShowLightSources(show_light_sources_);
			Scene::setAnimationSmoothness(animation_smoothness_);

			// Capping color (ColorRGBA + alpha) — ClippingPlane static.
			ColorRGBA capping;
			capping.set(capping_color_);
			ClippingPlane::getCappingColor() = capping;

			if (scene_ != nullptr)
			{
				// Offscreen rendering (enabled + resolution factor).
				scene_->setOffScreenRendering(offscreen_rendering_, (Size)offscreen_factor_);

				// FPS / preview toggles.
				scene_->setFPSEnabled(fps_enabled_);
				scene_->setPreview(preview_);

				// Smooth lines + downsampling are simple renderer-flag pushes.
				GLRenderer& renderer = scene_->getGLRenderer();
				renderer.setSmoothLines(smooth_lines_);
				scene_->setDownsamplingFactor(downsampling_factor_);
			}

			return true;
		}

		void StageController::invalidateDeclared_()
		{
			// §2a Stage row — controller-declared SCENE-STRUCTURAL side effects,
			// NOT a soft refresh and NOT a blanket rebuild. This is the ONE place
			// StageController declares its heavy invalidation (§2a): a
			// vertex-buffer-mode toggle or renderer switch deletes/rebuilds the
			// affected representations, then redrawAllRepresentations() runs;
			// the background/fog render state is refreshed afterwards.
			if (stage_ == nullptr) return;

			MainControl* mc = MainControl::getInstance(0);

			if (scene_ != nullptr)
			{
				GLRenderer& renderer = scene_->getGLRenderer();

				// Vertex buffers — with the documented rep-delete-on-toggle side
				// effect: when the buffer mode changes AND representations exist,
				// every representation is removed before the renderer flag flips
				// (mirrors stageSettings.C:285-298). Runs inside the apply guard
				// (apply() holds it across invalidateDeclared_()).
				if (mc != nullptr &&
				    vertex_buffers_enabled_ != renderer.vertexBuffersEnabled() &&
				    mc->getRepresentationManager().getNumberOfRepresentations() > 0)
				{
					mc->setStatusbarText(tr("Because of change in usage of vertex buffer, all Representations have to be deleted!"), true);
					RepresentationManager& pm = mc->getRepresentationManager();
					Size nr = pm.getNumberOfRepresentations();
					std::list<Representation*> reps = pm.getRepresentations();
					for (Position p = 0; p < nr; p++)
					{
						mc->remove(**reps.begin());
						reps.pop_front();
					}
				}

				renderer.enableVertexBuffers(vertex_buffers_enabled_);

				// Renderer switch (OpenGL / RTfact) — a scene-wide structural
				// change.
				scene_->switchRenderer(renderer_type_ == static_cast<int>(RenderSetup::RTFACT_RENDERER)
				                       ? RenderSetup::RTFACT_RENDERER
				                       : RenderSetup::OPENGL_RENDERER);

				// UFG-25 — refresh the per-renderer glClearColor + fog (the
				// renderers loop + updateGL the legacy applyPreferences path
				// did; just calling updateGL() repaints with the OLD cached
				// glClearColor). Covers background + fog.
				scene_->refreshSceneRenderState();
			}

			// redrawAllRepresentations() is the StageController-declared
			// scene-structural refresh (§2a Stage row) after a structural
			// change; harmless when nothing structural changed.
			if (mc != nullptr)
				mc->redrawAllRepresentations();
		}

		void StageController::reset()
		{
			// 999.64 — SINGLE-call reset to the METHOD-DEFINED defaults (NOT the
			// legacy two-step revert(); apply()). Restores the full non-stereo
			// render-config field set the controller owns to the constructor's
			// seed values: black background, no coordinate system, no fog, zero
			// eye/focal, perspective projection, no light-source markers, zero
			// animation smoothness, no offscreen rendering (factor 1), black
			// capping color, no FPS overlay, no preview, no vertex buffers, no
			// smooth lines, downsampling 1.0, OpenGL renderer. Mouse / wheel
			// sensitivity keep their "unset" sentinel (-1) so reset does NOT
			// clobber the user's interaction sensitivity (apply() skips sentinel
			// values). Both the Background and Stage Inspector sections drive this
			// controller, so a reset from either restores the shared stage state.
			// Emit the change notifications, then run the SINGLE §2 apply() (one
			// event, one reversible payload).
			background_color_        = QColor(Qt::black);
			show_coordinate_system_  = false;
			fog_intensity_           = 0.0f;
			eye_distance_            = 0.0f;
			focal_distance_          = 0.0f;
			perspective_projection_  = true;
			show_light_sources_      = false;
			animation_smoothness_    = 0.0f;
			offscreen_rendering_     = false;
			offscreen_factor_        = 1;
			capping_color_           = QColor(Qt::black);
			fps_enabled_             = false;
			preview_                 = false;
			vertex_buffers_enabled_  = false;
			smooth_lines_            = false;
			downsampling_factor_     = 1.0f;
			renderer_type_           = static_cast<int>(RenderSetup::OPENGL_RENDERER);

			Q_EMIT backgroundColorChanged(background_color_);
			Q_EMIT showCoordinateSystemChanged(show_coordinate_system_);
			Q_EMIT fogIntensityChanged(fog_intensity_);
			Q_EMIT eyeDistanceChanged(eye_distance_);
			Q_EMIT focalDistanceChanged(focal_distance_);
			Q_EMIT perspectiveProjectionChanged(perspective_projection_);
			Q_EMIT showLightSourcesChanged(show_light_sources_);
			Q_EMIT animationSmoothnessChanged(animation_smoothness_);
			Q_EMIT offScreenRenderingChanged(offscreen_rendering_);
			Q_EMIT offScreenFactorChanged(offscreen_factor_);
			Q_EMIT cappingColorChanged(capping_color_);
			Q_EMIT fpsEnabledChanged(fps_enabled_);
			Q_EMIT previewChanged(preview_);
			Q_EMIT vertexBuffersEnabledChanged(vertex_buffers_enabled_);
			Q_EMIT smoothLinesChanged(smooth_lines_);
			Q_EMIT downsamplingFactorChanged(downsampling_factor_);
			Q_EMIT rendererTypeChanged(renderer_type_);

			apply();
		}


		void StageController::setBackgroundColor(const QColor& c)
		{
			if (c == background_color_) return;
			background_color_ = c;
			Q_EMIT backgroundColorChanged(c);
		}

		void StageController::setShowCoordinateSystem(bool b)
		{
			if (b == show_coordinate_system_) return;
			show_coordinate_system_ = b;
			Q_EMIT showCoordinateSystemChanged(b);
		}

		void StageController::setFogIntensity(float v)
		{
			if (v == fog_intensity_) return;
			fog_intensity_ = v;
			Q_EMIT fogIntensityChanged(v);
		}

		void StageController::setEyeDistance(float v)
		{
			if (v == eye_distance_) return;
			eye_distance_ = v;
			Q_EMIT eyeDistanceChanged(v);
		}

		void StageController::setFocalDistance(float v)
		{
			if (v == focal_distance_) return;
			focal_distance_ = v;
			Q_EMIT focalDistanceChanged(v);
		}

		// --- Phase 999.58-02 — extended non-stereo render-config setters. ---

		void StageController::setPerspectiveProjection(bool b)
		{
			if (b == perspective_projection_) return;
			perspective_projection_ = b;
			Q_EMIT perspectiveProjectionChanged(b);
		}

		void StageController::setShowLightSources(bool b)
		{
			if (b == show_light_sources_) return;
			show_light_sources_ = b;
			Q_EMIT showLightSourcesChanged(b);
		}

		void StageController::setAnimationSmoothness(float v)
		{
			if (v == animation_smoothness_) return;
			animation_smoothness_ = v;
			Q_EMIT animationSmoothnessChanged(v);
		}

		void StageController::setOffScreenRendering(bool b)
		{
			if (b == offscreen_rendering_) return;
			offscreen_rendering_ = b;
			Q_EMIT offScreenRenderingChanged(b);
		}

		void StageController::setOffScreenFactor(int factor)
		{
			if (factor == offscreen_factor_) return;
			offscreen_factor_ = factor;
			Q_EMIT offScreenFactorChanged(factor);
		}

		void StageController::setCappingColor(const QColor& c)
		{
			if (c == capping_color_) return;
			capping_color_ = c;
			Q_EMIT cappingColorChanged(c);
		}

		void StageController::setFPSEnabled(bool b)
		{
			if (b == fps_enabled_) return;
			fps_enabled_ = b;
			Q_EMIT fpsEnabledChanged(b);
		}

		void StageController::setPreview(bool b)
		{
			if (b == preview_) return;
			preview_ = b;
			Q_EMIT previewChanged(b);
		}

		void StageController::setVertexBuffersEnabled(bool b)
		{
			if (b == vertex_buffers_enabled_) return;
			vertex_buffers_enabled_ = b;
			Q_EMIT vertexBuffersEnabledChanged(b);
		}

		void StageController::setSmoothLines(bool b)
		{
			if (b == smooth_lines_) return;
			smooth_lines_ = b;
			Q_EMIT smoothLinesChanged(b);
		}

		void StageController::setDownsamplingFactor(float v)
		{
			if (v == downsampling_factor_) return;
			downsampling_factor_ = v;
			Q_EMIT downsamplingFactorChanged(v);
		}

		void StageController::setRendererType(int type)
		{
			if (type == renderer_type_) return;
			renderer_type_ = type;
			Q_EMIT rendererTypeChanged(type);
		}

		void StageController::setMouseSensitivity(float v)
		{
			if (v == mouse_sensitivity_) return;
			mouse_sensitivity_ = v;
			Q_EMIT mouseSensitivityChanged(v);
		}

		void StageController::setMouseWheelSensitivity(float v)
		{
			if (v == mouse_wheel_sensitivity_) return;
			mouse_wheel_sensitivity_ = v;
			Q_EMIT mouseWheelSensitivityChanged(v);
		}

	} // namespace VIEW
} // namespace BALL

