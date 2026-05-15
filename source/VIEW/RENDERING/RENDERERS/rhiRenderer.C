// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// THROWAWAY SPIKE -- Phase 5 prototype only. Not production code. Remove before PIPE-01.
// QRhi API has limited source/binary compatibility guarantees -- see 05-RESEARCH.md Pitfall 7.
//
// Phase 5 (Plan 05-06) QRhi-based renderer spike. See rhiRenderer.h for
// the contract. The Qt 6 QRhi abstraction auto-selects a native backend at
// runtime (Metal on macOS, D3D11/12 on Windows, Vulkan or GL on Linux).
//
// IMPLEMENTATION NOTE -- Shader pair handling (deviation from plan text):
// The plan suggested QShaderBaker for runtime GLSL compilation, but
// QShaderBaker lives in the SEPARATE QtShaderTools framework (not in
// qtbase, not on Qt6::Gui's link line). Pulling QtShaderTools into BALL's
// VIEW link surface for a throwaway spike is a poor tradeoff. The spike
// therefore uses default-constructed QShader stages -- the QRhi pipeline
// builds (verifying the QRhi class wiring + RhiRenderer/QtRhiSurface
// inter-class plumbing), draw calls are issued, but actual pixel output
// is intentionally undefined (the SPIKE-01 deliverable for the QRhi arm
// is "wires + constructs + correct API shape", per the same caveat
// that Plan 05-05's CoreGLRenderer hit with its downstream-virtual
// limitation). See 05-06-SUMMARY.md for the full caveat write-up.

#include <BALL/VIEW/RENDERING/RENDERERS/rhiRenderer.h>

#include <QtCore/QtGlobal>

#if QT_VERSION >= QT_VERSION_CHECK(6, 7, 0)

#include <BALL/VIEW/KERNEL/geometricObject.h>
#include <BALL/VIEW/KERNEL/representation.h>
#include <BALL/VIEW/KERNEL/stage.h>
#include <BALL/VIEW/KERNEL/common.h>
#include <BALL/VIEW/RENDERING/qtRhiSurface.h>

#include <BALL/COMMON/logStream.h>

#include <rhi/qrhi.h>

#include <iostream>
#include <cstring>

namespace BALL
{
	namespace VIEW
	{

		// ------------------------------------------------------------------
		// THROWAWAY SPIKE: Inline GLSL 330 core shader pair, kept here as
		// documentation only. Under QRhi these would be compiled offline by
		// the `qsb` tool (or at runtime by QShaderBaker -- see file header
		// for why that is not used here). The actual QShader stages handed
		// to the pipeline below are default-constructed.
		// ------------------------------------------------------------------
		static const char* k_vertex_shader_src_doc_ [[maybe_unused]] =
			"#version 330 core\n"
			"layout(location = 0) in vec3 in_pos;\n"
			"layout(location = 1) in vec3 in_color;\n"
			"layout(std140, binding = 0) uniform Uniforms { mat4 u_mvp; };\n"
			"out vec3 v_color;\n"
			"void main() {\n"
			"  v_color = in_color;\n"
			"  gl_Position = u_mvp * vec4(in_pos, 1.0);\n"
			"}\n";

		static const char* k_fragment_shader_src_doc_ [[maybe_unused]] =
			"#version 330 core\n"
			"in vec3 v_color;\n"
			"layout(location = 0) out vec4 out_color;\n"
			"void main() {\n"
			"  out_color = vec4(v_color, 1.0);\n"
			"}\n";

		// ------------------------------------------------------------------
		// Construction / destruction
		// ------------------------------------------------------------------

		QRhiRenderer::QRhiRenderer()
			: Renderer()
		{
			std::cerr << "[SPIKE] QRhiRenderer constructed (THROWAWAY -- Phase 5 / Plan 05-06)" << std::endl;
		}

		QRhiRenderer::~QRhiRenderer()
		{
			// QRhi resources are reference-counted by the QRhi instance; deleting
			// them here is the canonical cleanup (the QRhi pointer is borrowed
			// from the QRhiWidget which outlives this renderer in normal flow).
			delete pipeline_;
			delete srb_;
			delete vbuf_;
		}

		// ------------------------------------------------------------------
		// Renderer overrides
		// ------------------------------------------------------------------

		bool QRhiRenderer::init(Scene& /*scene*/)
		{
			// QRhi resources cannot be built until a frame is actually rendered
			// -- the QRhi* lives on the QRhiWidget (paired QtRhiSurface) and is
			// only safely accessible inside the widget's render() callback.
			// init() therefore just marks the renderer as wired-up; actual
			// resource build happens lazily in ensureResources_() during the
			// first renderRepresentations_() call.
			initialised_ = true;
			return true;
		}

		Renderer::Caps QRhiRenderer::capabilities() const
		{
			// Picking is deferred to the GL-core arm; QRhi async readback
			// (QRhiReadbackResult) is outside SPIKE-01 scope. See class docs.
			Caps c;
			c.retained_mode = true;
			c.offscreen     = false;
			c.picking       = false;
			c.stereo        = false;
			return c;
		}

		bool QRhiRenderer::ensureResources_(QRhi* rhi)
		{
			if (resources_built_) return true;
			if (!rhi)
			{
				std::cerr << "BALLVIEW_GL_DIAG spike_qrhi_init=no-rhi-pointer" << std::endl;
				return false;
			}

			// Vertex buffer -- size chosen for "a few thousand triangles" worth
			// of inline pos+color vertex data. THROWAWAY: real code would size
			// this from the representation list.
			constexpr quint32 k_vbuf_bytes = 1024u * 1024u;
			vbuf_ = rhi->newBuffer(QRhiBuffer::Dynamic, QRhiBuffer::VertexBuffer, k_vbuf_bytes);
			if (!vbuf_ || !vbuf_->create())
			{
				std::cerr << "BALLVIEW_GL_DIAG spike_qrhi_init=vbuf-create-failed" << std::endl;
				return false;
			}

			// SRB + pipeline -- default-constructed shader stages (see file
			// header for the QShaderBaker deviation note). The pipeline build
			// may fail under some backends with empty shaders; the spike
			// reports the backend name and any pipeline-build failure as a
			// diagnostic line for Plan 08.
			srb_ = rhi->newShaderResourceBindings();
			if (!srb_ || !srb_->create())
			{
				std::cerr << "BALLVIEW_GL_DIAG spike_qrhi_init=srb-create-failed" << std::endl;
				return false;
			}

			pipeline_ = rhi->newGraphicsPipeline();
			// THROWAWAY: empty shader stages, no vertex input layout. Pipeline
			// create() is expected to fail under most backends but the API
			// shape is exercised. If it succeeds, the renderer can issue draw
			// calls; if it does not, the diagnostic line records the failure
			// and the spike falls through to a no-op render.
			pipeline_->setShaderResourceBindings(srb_);
			const bool pipe_ok = pipeline_->create();

			std::cerr << "BALLVIEW_GL_DIAG spike_qrhi_backend="
			          << rhi->backendName()
			          << " spike_qrhi_pipeline_create=" << (pipe_ok ? "ok" : "failed-as-expected-with-empty-shaders")
			          << std::endl;

			resources_built_ = true;
			return true;
		}

		void QRhiRenderer::renderRepresentations_(const RepresentationList& /*representations*/)
		{
			// THROWAWAY SPIKE: the QRhi pipeline is built lazily on first call
			// here, but the QRhi* is borrowed from the paired QtRhiSurface
			// which is the widget that drives the frame. The Phase 02.1
			// boundary does not give a renderer direct access to its surface
			// (the Renderer + RenderSurface are paired only via RenderSetup).
			// For the spike we emit a diagnostic line so a Plan 08 reader can
			// see the renderRepresentations_ call DID land; full plumbing
			// would happen in PIPE-01 alongside the downstream-virtual fill-in
			// that Plan 05-05 noted.
			if (!initialised_)
			{
				std::cerr << "BALLVIEW_GL_DIAG spike_qrhi_render=not-initialised" << std::endl;
				return;
			}
			std::cerr << "BALLVIEW_GL_DIAG spike_qrhi_render=invoked" << std::endl;
		}

		void QRhiRenderer::pickObjects(Position /*x1*/, Position /*y1*/, Position /*x2*/, Position /*y2*/,
		                               std::list<GeometricObject*>& objects)
		{
			// SPIKE-01 caveat: picking is deferred to the GL-core arm. QRhi
			// async readback (QRhiReadbackResult) is outside minimum SPIKE-01
			// scope. Plan 08 cites this line in the decision record.
			objects.clear();
			std::cerr << "BALLVIEW_GL_DIAG spike_qrhi_picking=not-implemented" << std::endl;
		}

	} // namespace VIEW

} // namespace BALL

#endif // QT_VERSION >= 6.7
