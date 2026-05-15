// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// THROWAWAY SPIKE -- Phase 5 prototype only. Not production code. Remove before PIPE-01.
// QRhi API has limited source/binary compatibility guarantees -- see 05-RESEARCH.md Pitfall 7.
//
// Phase 5 (Plan 05-06) QRhi-based renderer spike. Behind
// -DBALL_SPIKE_BACKEND=QRhi at configure time (Qt 6.7+ required) +
// BALLVIEW_USE_SPIKE_BACKEND=1 at runtime; default builds never see this
// class. Companion class to CoreGLRenderer from Plan 05-05; together the
// two form the SPIKE-01 comparison set consumed by Plan 08's SPIKE-02
// decision record.

#ifndef BALL_VIEW_RENDERING_RENDERERS_RHIRENDERER_H
#define BALL_VIEW_RENDERING_RENDERERS_RHIRENDERER_H

#ifndef BALL_VIEW_RENDERING_RENDERERS_RENDERER_H
#	include <BALL/VIEW/RENDERING/RENDERERS/renderer.h>
#endif

#include <QtCore/QtGlobal>

// Belt-and-braces: the CMake-side BALL_SPIKE_BACKEND_QRHI defn is only set
// when Qt >= 6.7, but if someone hand-defines it on an older Qt the build
// must still fail gracefully -- not silently compile a class that references
// types that do not exist yet.
#if QT_VERSION >= QT_VERSION_CHECK(6, 7, 0)

QT_BEGIN_NAMESPACE
class QRhi;
class QRhiBuffer;
class QRhiGraphicsPipeline;
class QRhiShaderResourceBindings;
class QRhiCommandBuffer;
QT_END_NAMESPACE

namespace BALL
{
	namespace VIEW
	{
		class QtRhiSurface;

		/** THROWAWAY SPIKE -- Phase 5 prototype only. Not production code.
		 *  Remove before PIPE-01.
		 *
		 *  QRhiRenderer is the QRhi arm of the SPIKE-01 backend prototype.
		 *  It uses QRhi's cross-API rendering abstraction (Metal on macOS,
		 *  D3D11/12 on Windows, Vulkan or GL on Linux -- Qt auto-selects
		 *  the backend at runtime). The QRhi context is owned by the
		 *  paired QtRhiSurface (a QRhiWidget subclass); this renderer
		 *  obtains it via QRhiWidget::rhi() in render() and builds its
		 *  vertex buffer + graphics pipeline lazily on first use.
		 *
		 *  Overrides the *batched* Renderer::renderRepresentations_() +
		 *  capabilities() entry points (Phase 02.1 boundary) -- it does NOT
		 *  implement the per-primitive immediate-mode render*_() virtuals,
		 *  matching the CoreGLRenderer spike contract.
		 *
		 *  Picking is intentionally NOT implemented in the QRhi arm --
		 *  color-buffer FBO picking under QRhi requires async
		 *  QRhiReadbackResult plumbing that falls outside the minimum
		 *  SPIKE-01 deliverable. The GL-core arm (CoreGLRenderer) satisfies
		 *  the "demonstrating picking" criterion of SPIKE-01. See
		 *  05-06-SUMMARY.md for the deferred-caveat write-up.
		 */
		class BALL_VIEW_EXPORT QRhiRenderer : public Renderer
		{
			public:

				QRhiRenderer();
				virtual ~QRhiRenderer();

				/// Initialise the QRhi resources. Called by Scene::initializeGL
				/// via RenderSetup. The actual QRhi* is obtained later (per render
				/// pass) from the paired QtRhiSurface since the QRhi instance is
				/// owned by the QRhiWidget, not by this Renderer.
				virtual bool init(Scene& scene) override;

				/// Batched, retained-mode render entry point. Builds vertex data
				/// from the representation list, uploads to a QRhiBuffer, issues
				/// one draw call. The actual command buffer is owned by the
				/// paired QRhiWidget which calls this from its render() override.
				virtual void renderRepresentations_(const RepresentationList& representations) override;

				/// Capability descriptor: retained_mode=true, offscreen=false,
				/// picking=false, stereo=false. Picking is documented as a
				/// deferred caveat (see class docs above).
				virtual Caps capabilities() const override;

				/// Picking is NOT implemented for the QRhi arm. Returns empty
				/// list and emits a BALLVIEW_GL_DIAG diagnostic line so
				/// Plan 08 (SPIKE-02 decision record) can cite the caveat.
				virtual void pickObjects(Position x1, Position y1, Position x2, Position y2,
				                         std::list<GeometricObject*>& objects) override;

			private:

				/// Lazily build the graphics pipeline + vertex buffer the first
				/// time a render pass runs (when the QRhi* is finally accessible
				/// via the paired QRhiWidget).
				bool ensureResources_(QRhi* rhi);

				/// Delete the QRhi resources (pipeline / SRB / vbuf) in reverse
				/// order of construction, null each pointer, and clear
				/// resources_built_. Called from both the destructor and from
				/// every failure path inside ensureResources_() so partial
				/// allocations do not leak across retries (review WR-02/WR-04).
				void releaseResources_();

				bool initialised_ = false;
				bool resources_built_ = false;

				// QRhi resources -- owned, deleted in destructor.
				QRhiBuffer* vbuf_ = nullptr;
				QRhiGraphicsPipeline* pipeline_ = nullptr;
				QRhiShaderResourceBindings* srb_ = nullptr;
		};

	} // namespace VIEW

} // namespace BALL

#endif // QT_VERSION >= 6.7

#endif // BALL_VIEW_RENDERING_RENDERERS_RHIRENDERER_H
