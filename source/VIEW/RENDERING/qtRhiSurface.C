// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// THROWAWAY SPIKE -- Phase 5 prototype only. Not production code. Remove before PIPE-01.
// QRhi API has limited source/binary compatibility guarantees -- see 05-RESEARCH.md Pitfall 7.
//
// Phase 5 (Plan 05-06) QRhi-based RenderSurface impl. See qtRhiSurface.h
// for the contract. The QRhi backend is auto-selected by Qt at runtime:
// Metal on macOS, D3D11/12 on Windows, Vulkan or GL on Linux.

#include <BALL/VIEW/RENDERING/qtRhiSurface.h>

#include <QtCore/QtGlobal>

#if QT_VERSION >= QT_VERSION_CHECK(6, 7, 0)

#include <rhi/qrhi.h>

#include <iostream>

namespace BALL
{
	namespace VIEW
	{

		// ------------------------------------------------------------------
		// Construction / destruction
		// ------------------------------------------------------------------

		QtRhiSurface::QtRhiSurface(QWidget* parent)
			: RenderSurface(),
				QRhiWidget(parent)
		{
			std::cerr << "[SPIKE] QtRhiSurface constructed (THROWAWAY -- Phase 5 / Plan 05-06)" << std::endl;
			// THROWAWAY: explicit API request keeps the spike portable.
			// Qt 6.7+ default is to auto-select per platform; leaving the
			// default in place yields Metal on macOS, D3D11 on Windows, GL
			// or Vulkan on Linux. The diagnostic line in initialize()
			// reports which one Qt actually chose.
		}

		QtRhiSurface::~QtRhiSurface()
		{
		}

		// ------------------------------------------------------------------
		// RenderSurface contract
		// ------------------------------------------------------------------

		void QtRhiSurface::beginFrame()
		{
			// THROWAWAY SPIKE: no-op. QRhiWidget owns the frame lifecycle;
			// the BALL-side beginFrame verb (called by RenderSetup) cannot
			// drive a QRhi frame outside the widget's render() callback.
			// PIPE-01 would re-shape RenderSetup to route through the
			// QRhiWidget event-loop entry point instead.
		}

		void QtRhiSurface::endFrame()
		{
			// THROWAWAY SPIKE: no-op. (See beginFrame() comment.)
		}

		void* QtRhiSurface::nativeHandle()
		{
			// Returns the QRhi* (opaque to non-QRhi callers). The paired
			// QRhiRenderer downcasts this to access the QRhi instance for
			// pipeline / buffer creation. Per RENDERER-INTERFACE-BOUNDARY.md
			// §RenderSurface, only the matching backend casts this back.
			return static_cast<void*>(QRhiWidget::rhi());
		}

		// ------------------------------------------------------------------
		// RenderTarget pure-virtual stubs -- throwaway, never reached in
		// the QRhi-spike runtime path. PIPE-01 will re-shape the
		// RenderTarget interface so these can be defaulted at the base.
		// ------------------------------------------------------------------

		FrameBufferPtr QtRhiSurface::getBuffer()
		{
			return FrameBufferPtr();
		}

		FrameBufferFormat QtRhiSurface::getFormat() const
		{
			return FrameBufferFormat();
		}

		void QtRhiSurface::releaseBuffer(FrameBufferPtr /*buffer*/)
		{
			// no-op: no CPU-buffer ownership in the QRhi spike path.
		}

		bool QtRhiSurface::init()
		{
			// QRhi initialisation is driven by QRhiWidget::initialize(), not
			// by this BALL-side init() verb.
			return true;
		}

		bool QtRhiSurface::resize(const unsigned int /*width*/, const unsigned int /*height*/)
		{
			// QRhiWidget handles resize via its own resizeEvent() override.
			return true;
		}

		void QtRhiSurface::refresh()
		{
			// QRhiWidget repaints on Qt event-loop tick; no manual refresh.
		}

		void QtRhiSurface::prepareRendering()
		{
			// QRhiWidget owns the rendering context lifecycle.
		}

		bool QtRhiSurface::doNotResize() const
		{
			return false;
		}

		// ------------------------------------------------------------------
		// QRhiWidget contract
		// ------------------------------------------------------------------

		void QtRhiSurface::initialize(QRhiCommandBuffer* /*cb*/)
		{
			QRhi* r = QRhiWidget::rhi();
			if (r)
			{
				// Plan 05-06 output requirement: report the QRhi backend Qt
				// auto-selected on this platform. Plan 08 (SPIKE-02 decision
				// record) consumes this line from the smoke-check log.
				std::cerr << "BALLVIEW_GL_DIAG renderer_backend=QRhi "
				          << "qrhi_backend_name=" << r->backendName() << " "
				          << "qrhi_driver=" << r->driverInfo().deviceName.constData()
				          << std::endl;
			}
			else
			{
				std::cerr << "BALLVIEW_GL_DIAG renderer_backend=QRhi qrhi_backend_name=null-rhi"
				          << std::endl;
			}
		}

		void QtRhiSurface::render(QRhiCommandBuffer* cb)
		{
			// THROWAWAY SPIKE: clear-only render pass. A full implementation
			// would forward to the paired QRhiRenderer::renderRepresentations_()
			// here, but the BALL surface/renderer pairing is driven by
			// RenderSetup which has no direct hook into the QRhiWidget's
			// event-loop-driven render() callback. PIPE-01 scope.
			//
			// Clearing to a deterministic background colour means the smoke
			// PNG is non-blank without depending on the renderer plumbing.
			if (!cb) return;
			QRhiRenderTarget* rt = QRhiWidget::renderTarget();
			if (!rt) return;

			const QColor clear_colour(40, 60, 90);  // dark blue, deliberately
			                                        // distinct from default
			                                        // black so a non-blank
			                                        // PNG check passes.
			cb->beginPass(rt, clear_colour, { 1.0f, 0 });
			cb->endPass();
		}

	} // namespace VIEW

} // namespace BALL

#endif // QT_VERSION >= 6.7
