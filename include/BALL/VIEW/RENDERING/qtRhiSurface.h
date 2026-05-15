// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// THROWAWAY SPIKE -- Phase 5 prototype only. Not production code. Remove before PIPE-01.
// QRhi API has limited source/binary compatibility guarantees -- see 05-RESEARCH.md Pitfall 7.
//
// Phase 5 (Plan 05-06) QRhi-based RenderSurface impl. Wraps QRhiWidget
// (Qt 6.7+). Behind -DBALL_SPIKE_BACKEND=QRhi at configure time; default
// builds never see this class. Companion to GLRenderWindow which plays
// the same role for the legacy GL path.

#ifndef BALL_VIEW_RENDERING_QTRHISURFACE_H
#define BALL_VIEW_RENDERING_QTRHISURFACE_H

#ifndef BALL_COMMON_GLOBAL_H
#	include <BALL/COMMON/global.h>
#endif

#ifndef BALL_VIEW_RENDERING_RENDERSURFACE_H
#	include <BALL/VIEW/RENDERING/renderSurface.h>
#endif

#include <QtCore/QtGlobal>

// QRhiWidget requires Qt 6.7+. The CMake-side floor check is the primary
// guard (BALL_SPIKE_BACKEND_QRHI is never defined on older Qt); this
// QT_VERSION_CHECK is belt-and-braces so a hand-defined macro on older Qt
// fails to declare the class rather than producing cryptic errors deep
// inside the widget header.
#if QT_VERSION >= QT_VERSION_CHECK(6, 7, 0)

#include <QtWidgets/QRhiWidget>

QT_BEGIN_NAMESPACE
class QRhi;
class QRhiCommandBuffer;
QT_END_NAMESPACE

namespace BALL
{
	namespace VIEW
	{
		class QRhiRenderer;

		/** THROWAWAY SPIKE -- Phase 5 prototype only. Not production code.
		 *  Remove before PIPE-01.
		 *
		 *  QtRhiSurface is the QRhi arm of the SPIKE-01 surface prototype.
		 *  It multiply-inherits from RenderSurface (Phase 02.1 boundary)
		 *  and QRhiWidget (Qt 6.7+), mirroring the GLRenderWindow pattern
		 *  (RenderWindow + QOpenGLWidget). The QRhiWidget IS the surface --
		 *  it owns the QRhi* instance, the command buffer lifecycle, and
		 *  the underlying native window surface.
		 *
		 *  The RenderSurface verbs (beginFrame / endFrame) are deliberate
		 *  no-ops for this surface: QRhiWidget drives the frame lifecycle
		 *  via its initialize() / render() virtuals, which are called by
		 *  Qt's main event loop -- not by the BALL RenderSetup. nativeHandle()
		 *  exposes the QRhi* pointer (opaque to non-QRhi callers) so the
		 *  paired QRhiRenderer can build its pipeline lazily on first
		 *  render() invocation.
		 */
		class BALL_VIEW_EXPORT QtRhiSurface
			: public RenderSurface,
				public QRhiWidget
		{
			public:

				explicit QtRhiSurface(QWidget* parent = nullptr);
				virtual ~QtRhiSurface();

				// ----- RenderSurface contract -------------------------------

				/// THROWAWAY SPIKE: no-op. QRhiWidget drives the frame
				/// lifecycle from Qt's event loop; the BALL-side
				/// beginFrame/endFrame verbs (called by RenderSetup) cannot
				/// directly drive a QRhi frame outside the widget's render()
				/// callback. See file header for the structural caveat.
				virtual void beginFrame() override;

				/// THROWAWAY SPIKE: no-op. (See beginFrame() comment.)
				virtual void endFrame() override;

				/// Returns the QRhi* pointer cast to void*. Only the paired
				/// QRhiRenderer downcasts this; per
				/// RENDERER-INTERFACE-BOUNDARY.md the handle is opaque to
				/// non-matching backends.
				virtual void* nativeHandle() override;

				// ----- RenderTarget contract (inherited via RenderSurface) --
				//
				// RenderSurface inherits RenderTarget's CPU-buffer-shaped
				// pure virtuals (getBuffer / getFormat / releaseBuffer /
				// init / resize / refresh / prepareRendering / doNotResize).
				// These are shaped for the CPU-buffer / raytracer flow and
				// have no meaningful semantics for a QRhi-driven surface
				// (QRhi owns its own surface texture; there is no BALL-side
				// FrameBuffer). They are implemented here as throwaway
				// no-op / never-used stubs so the class is concrete.
				// PIPE-01 will re-shape the RenderTarget interface to make
				// these optional rather than pure.

				virtual FrameBufferPtr getBuffer() override;
				virtual FrameBufferFormat getFormat() const override;
				virtual void releaseBuffer(FrameBufferPtr buffer) override;
				virtual bool init() override;
				virtual bool resize(const unsigned int width, const unsigned int height) override;
				virtual void refresh() override;
				virtual void prepareRendering() override;
				virtual bool doNotResize() const override;

			protected:

				// ----- QRhiWidget contract ----------------------------------

				/// Called once after the QRhi* is ready. Emits a
				/// BALLVIEW_GL_DIAG diagnostic line so Plan 08 can observe
				/// which native backend (Metal / Vulkan / D3D / GL) Qt
				/// auto-selected.
				virtual void initialize(QRhiCommandBuffer* cb) override;

				/// Per-frame render callback driven by Qt's event loop. The
				/// spike does NOT route to QRhiRenderer here directly --
				/// the renderer is constructed by RendererFactory and wired
				/// via RenderSetup, but the structural plumbing to drive it
				/// from inside this callback is PIPE-01 scope (matches
				/// Plan 05-05's CoreGLRenderer downstream-virtual caveat).
				/// The spike simply clears the framebuffer so a non-blank
				/// PNG smoke check can observe a deterministic background.
				virtual void render(QRhiCommandBuffer* cb) override;
		};

	} // namespace VIEW

} // namespace BALL

#endif // QT_VERSION >= 6.7

#endif // BALL_VIEW_RENDERING_QTRHISURFACE_H
