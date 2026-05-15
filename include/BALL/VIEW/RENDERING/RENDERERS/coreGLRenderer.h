// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// THROWAWAY SPIKE — Phase 5 prototype only. Not production code. Remove before PIPE-01.
//
// Phase 5 (Plan 05-05) GL-core profile renderer spike. Behind
// -DBALL_SPIKE_BACKEND=GLCore at configure time + BALLVIEW_USE_SPIKE_BACKEND=1
// at runtime; default builds never see this class. Implements the SPIKE-01
// deliverable: render-the-demo-molecule + color-buffer-FBO picking + QPainter
// text overlay (via the existing GLRenderWindow paintGL path).

#ifndef BALL_VIEW_RENDERING_RENDERERS_COREGLRENDERER_H
#define BALL_VIEW_RENDERING_RENDERERS_COREGLRENDERER_H

#ifndef BALL_VIEW_RENDERING_RENDERERS_RENDERER_H
#	include <BALL/VIEW/RENDERING/RENDERERS/renderer.h>
#endif

#include <vector>
#include <unordered_map>

class QOpenGLFunctions_3_2_Core;

namespace BALL
{
	namespace VIEW
	{
		/** THROWAWAY SPIKE — Phase 5 prototype only. Not production code.
		 *  Remove before PIPE-01.
		 *
		 *  CoreGLRenderer is the GL-core arm of the SPIKE-01 backend prototype.
		 *  It uses GL 3.2+ core-profile entry points (VAO/VBO upload + GLSL
		 *  shaders, no fixed-function), implements color-buffer FBO picking,
		 *  and leaves the framebuffer in a state that the existing QPainter
		 *  text-overlay path in GLRenderWindow::paintGL() can composite on top of.
		 *
		 *  This renderer overrides the *batched* Renderer::renderRepresentations_()
		 *  + capabilities() entry points (Phase 02.1 boundary) — it does NOT
		 *  implement the per-primitive immediate-mode render*_() virtuals.
		 */
		class BALL_VIEW_EXPORT CoreGLRenderer : public Renderer
		{
			public:

				CoreGLRenderer();
				virtual ~CoreGLRenderer();

				/// Initialise the GL-core context: create VAO/VBOs, compile shader pair,
				/// create the picking FBO. Called by Scene::initializeGL via RenderSetup.
				virtual bool init(Scene& scene) override;

				/// Batched, retained-mode render entry point. Iterates the
				/// representation list, builds VBO data once per call, issues one
				/// draw call per representation. Restores GL state (depth test on,
				/// FBO 0 bound) before returning so the QPainter overlay in
				/// GLRenderWindow::paintGL() composites cleanly on top.
				virtual void renderRepresentations_(const RepresentationList& representations) override;

				/// Capability descriptor: retained_mode=true, offscreen=false,
				/// picking=true, stereo=false.
				virtual Caps capabilities() const override;

				/// Color-buffer FBO + glReadPixels picking. Renders the scene a
				/// second time into the picking FBO with per-object IDs encoded
				/// in the colour channel, then reads the pixel under the cursor
				/// to recover the picked GeometricObject*.
				virtual void pickObjects(Position x1, Position y1, Position x2, Position y2,
				                         std::list<GeometricObject*>& objects) override;

			private:

				/// Compile + link the GLSL 330-core shader pair. Returns true on success.
				bool initShaders_();

				/// Create the FBO + colour attachment + depth attachment used for picking.
				bool initPickingFBO_(int width, int height);

				/// Resize the picking FBO when the viewport changes.
				void resizePickingFBO_(int width, int height);

				/// Get the GL 3.2 core function table for the current QOpenGLContext.
				/// Returns nullptr if the context is not core 3.2+ (the spike then
				/// gracefully falls through to a no-op, leaving the legacy compat
				/// context to drive the QPainter overlay layer).
				QOpenGLFunctions_3_2_Core* glFns_();

				bool initialised_ = false;

				// GL object names — populated by init().
				unsigned int program_id_   = 0;     // shader program handle
				unsigned int vao_id_       = 0;     // shared VAO
				unsigned int vbo_id_       = 0;     // shared per-frame VBO
				unsigned int picking_fbo_  = 0;     // FBO for picking pass
				unsigned int picking_tex_  = 0;     // R32UI colour attachment
				unsigned int picking_depth_ = 0;    // depth renderbuffer

				int picking_fbo_w_ = 0;
				int picking_fbo_h_ = 0;

				// Uniform locations cached at link time.
				int uloc_mvp_ = -1;
				int uloc_pick_mode_ = -1;
				int uloc_picking_id_ = -1;

				// Per-rep ID -> GeometricObject* map, rebuilt every frame.
				std::unordered_map<unsigned int, GeometricObject*> id_to_object_;
		};

	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_RENDERING_RENDERERS_COREGLRENDERER_H
