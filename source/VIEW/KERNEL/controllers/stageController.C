// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 — StageController implementation.
//

#include <BALL/VIEW/KERNEL/controllers/stageController.h>


#include <BALL/VIEW/KERNEL/stage.h>
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

// std::clamp is used by colorRGBAToQColor_ below.
#include <algorithm>

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
		}

		StageController::StageController(Stage* stage, Scene* scene, QObject* parent)
			: QObject(parent),
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
				mouse_wheel_sensitivity_(-1.0f),  // sentinel: unset
				applying_(false)
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

		void StageController::apply()
		{
			// Phase 999.58-02 — non-stereo stage render-config cut-over
			// COMPLETE. This controller now owns the full field set the
			// legacy StageSettings::apply() (stageSettings.C:233-309) wrote:
			// background + coordinate-system + fog + eye/focal (since 999.44),
			// plus mouse/wheel sensitivity, projection mode, show-light-sources,
			// animation smoothness, offscreen rendering, capping color,
			// FPS-enabled, preview, vertex buffers (incl. the rep-delete-on-
			// toggle side effect), smooth lines, downsampling, and the
			// OpenGL/RTfact renderer switch. swap-side-by-side is deliberately
			// NOT written here — StereoController owns it (avoids a double-write
			// race; see stereoController.C). Scene::applyPreferences() now drives
			// the stage through this method instead of stage_settings_->apply().
			if (stage_ == nullptr)
			{
				Log.warn() << "[StageController::apply] no Stage attached — skipping." << std::endl;
				return;
			}

			// Phase 999.58-02 — defer while MainControl is busy (mirrors
			// StereoController::apply()). The extended apply() can trigger a
			// renderer switch and a bulk representation rebuild (vertex-buffer
			// toggle), so it must not run mid-busy-cycle (T-99858-04).
			MainControl* mc = MainControl::getInstance(0);
			if (mc != nullptr && mc->isBusy())
			{
				Log.info() << "[StageController::apply] MainControl busy — deferring." << std::endl;
				return;
			}

			// v1.7.x-24 — re-entrancy shield. If a notification triggered by
			// this apply() (e.g. the scene refresh below) synchronously
			// re-enters apply(), bail rather than re-running the mutation —
			// this is the cascade class behind the v1.7.x-13 freeze. The RAII
			// guard clears the flag on every exit path. The vertex-buffer
			// rep-delete side effect runs INSIDE this guard (T-99858-04).
			if (applying_) return;
			ControllerApplyGuard apply_guard(applying_);

			// background_color_ is QColor; Stage stores ColorRGBA.
			// ColorRGBA::set(const QColor&) handles the 0..255 →
			// 0..1 (ColorUnit) conversion + alpha.
			ColorRGBA c;
			c.set(background_color_);
			stage_->setBackgroundColor(c);

			stage_->showCoordinateSystem(show_coordinate_system_);
			stage_->setFogIntensity(fog_intensity_);
			stage_->setEyeDistance(eye_distance_);
			stage_->setFocalDistance(focal_distance_);

			// --- Phase 999.58-02: the extended non-stereo render config ---
			// Each call routes through the SAME backend the legacy
			// StageSettings::apply() used (stageSettings.C:238-308).

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

				// Vertex buffers — with the documented rep-delete-on-toggle side
				// effect: when the buffer mode changes AND representations exist,
				// every representation is removed before the renderer flag flips
				// (mirrors stageSettings.C:285-298). Runs inside the apply guard.
				GLRenderer& renderer = scene_->getGLRenderer();
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
				renderer.setSmoothLines(smooth_lines_);

				// Downsampling factor.
				scene_->setDownsamplingFactor(downsampling_factor_);

				// Renderer switch (OpenGL / RTfact).
				scene_->switchRenderer(renderer_type_ == static_cast<int>(RenderSetup::RTFACT_RENDERER)
				                       ? RenderSetup::RTFACT_RENDERER
				                       : RenderSetup::OPENGL_RENDERER);
			}

			// UFG-25 (rc4 → rc5 → rc5-followup) — Stage::setBackgroundColor()
			// only stores the new value in the Stage struct. The
			// actual `glClearColor()` requires a separate call to
			// `RenderSetup::updateBackgroundColor()` per renderer,
			// and the fog intensity needs `setFogIntensity()` on the
			// GL renderer (these are what the legacy Stage Preferences
			// dialog does in `Scene::applyPreferences` lines
			// 1142-1155). Just calling `updateGL()` repaints with the
			// OLD glClearColor cached on the renderer.
			//
			// `Scene::refreshSceneRenderState()` is the new public
			// helper that does the renderers loop + updateGL — mirroring
			// the applyPreferences path. Covers background + fog;
			// coordinate-system toggle propagates via the existing
			// stage_->showCoordinateSystem() above.
			if (scene_ != nullptr)
				scene_->refreshSceneRenderState();

			// Emit appliedStub so callers wired during the migration
			// window keep observing the apply signal. (Renamed in the
			// 999.48 cleanup to applied().)
			Q_EMIT appliedStub();
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

