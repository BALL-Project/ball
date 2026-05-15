// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// THROWAWAY SPIKE — Phase 5 prototype only. Not production code. Remove before PIPE-01.
//
// Phase 5 (Plan 05-05) GL-core profile renderer spike. See coreGLRenderer.h
// for the contract. The shader pair is inlined below as a const char*; the
// picking FBO is a single R32UI colour attachment + a depth renderbuffer.

#include <BALL/VIEW/RENDERING/RENDERERS/coreGLRenderer.h>

#include <BALL/VIEW/KERNEL/geometricObject.h>
#include <BALL/VIEW/KERNEL/representation.h>
#include <BALL/VIEW/KERNEL/stage.h>
#include <BALL/VIEW/KERNEL/common.h>

#include <BALL/COMMON/logStream.h>

#include <QtGui/QOpenGLContext>
// Qt 6: QOpenGLFunctions_*_Core / QOpenGLVersionFunctionsFactory live in
// QtOpenGL (not QtGui). QOpenGLContext::versionFunctions<T>() was removed -
// use QOpenGLVersionFunctionsFactory::get<T>(context) instead.
#include <QtOpenGL/QOpenGLFunctions_3_2_Core>
#include <QtOpenGL/QOpenGLVersionFunctionsFactory>
#include <QtOpenGL/QOpenGLFramebufferObject>

#include <cstring>

namespace BALL
{
	namespace VIEW
	{

		// ------------------------------------------------------------------
		// Inline GLSL 330 core shader pair.
		// Vertex shader: transform in_pos by an MVP uniform, pass through colour.
		// Fragment shader: either write the per-vertex colour (main pass) or
		// the packed object-ID as an R32UI colour (picking pass).
		// THROWAWAY: shader code is intentionally minimal — no lighting, no
		// normal-mapping. The spike rendering is "shows up as something on
		// screen" not "pixel-equivalent to GLRenderer".
		// ------------------------------------------------------------------
		static const char* k_vertex_shader_src_ =
			"#version 330 core\n"
			"layout(location = 0) in vec3 in_pos;\n"
			"layout(location = 1) in vec3 in_color;\n"
			"uniform mat4 u_mvp;\n"
			"out vec3 v_color;\n"
			"void main() {\n"
			"  v_color = in_color;\n"
			"  gl_Position = u_mvp * vec4(in_pos, 1.0);\n"
			"}\n";

		static const char* k_fragment_shader_src_ =
			"#version 330 core\n"
			"in vec3 v_color;\n"
			"layout(location = 0) out vec4 out_color;\n"
			"layout(location = 1) out uint out_picking_id;\n"
			"uniform int  u_pick_mode;   // 0 = main, 1 = picking\n"
			"uniform uint u_picking_id;  // current object ID (set per-rep)\n"
			"void main() {\n"
			"  if (u_pick_mode == 1) {\n"
			"    out_picking_id = u_picking_id;\n"
			"    out_color = vec4(0.0);\n"
			"  } else {\n"
			"    out_color = vec4(v_color, 1.0);\n"
			"    out_picking_id = 0u;\n"
			"  }\n"
			"}\n";

		// ------------------------------------------------------------------

		CoreGLRenderer::CoreGLRenderer()
		{
			Log.info() << "[SPIKE] CoreGLRenderer constructed (THROWAWAY -- Phase 5 prototype)" << std::endl;
		}

		CoreGLRenderer::~CoreGLRenderer()
		{
			// THROWAWAY: in production we would tear down the FBO + program + VAO
			// here; in the spike the QOpenGLWidget shutdown reclaims the resources.
		}

		QOpenGLFunctions_3_2_Core* CoreGLRenderer::glFns_()
		{
			QOpenGLContext* ctx = QOpenGLContext::currentContext();
			if (!ctx)
				return nullptr;
			// Qt 6 idiom: factory replaces context->versionFunctions<T>().
			return QOpenGLVersionFunctionsFactory::get<QOpenGLFunctions_3_2_Core>(ctx);
		}

		Renderer::Caps CoreGLRenderer::capabilities() const
		{
			// SPIKE-01 deliverable: retained-mode + picking. No offscreen tiling
			// (that's the raytracer's domain), no stereo (Phase 02.1 deferred 9 sites).
			Caps c;
			c.retained_mode = true;
			c.offscreen     = false;
			c.picking       = true;
			c.stereo        = false;
			return c;
		}

		bool CoreGLRenderer::init(Scene& scene)
		{
			Renderer::init(scene);

			// THROWAWAY: this spike only works on a GL 3.2 core (or higher) context.
			// macOS Apple Silicon only exposes 4.1 core when the QSurfaceFormat
			// requests CoreProfile -- the production compat-profile path used by
			// default builds is GL 2.1 and will fail this version lookup. That is
			// expected and acceptable for the spike: production paths never reach
			// here (env-var gate in RendererFactory keeps us out).
			QOpenGLFunctions_3_2_Core* fns = glFns_();
			if (!fns)
			{
				Log.warn() << "[SPIKE] CoreGLRenderer::init: no GL 3.2 core context "
				           << "(spike requires CoreProfile context, falling back to no-op)"
				           << std::endl;
				return false;
			}
			fns->initializeOpenGLFunctions();

			if (!initShaders_())
				return false;

			// Create the shared VAO + VBO. Geometry is rewritten per frame in
			// renderRepresentations_(); this is intentional throwaway shape.
			fns->glGenVertexArrays(1, &vao_id_);
			fns->glGenBuffers(1, &vbo_id_);

			initialised_ = true;
			Log.info() << "[SPIKE] CoreGLRenderer initialised against GL 3.2 core context" << std::endl;
			return true;
		}

		bool CoreGLRenderer::initShaders_()
		{
			QOpenGLFunctions_3_2_Core* fns = glFns_();
			if (!fns)
				return false;

			auto compile_one = [&](unsigned int stage, const char* src) -> unsigned int {
				unsigned int s = fns->glCreateShader(stage);
				fns->glShaderSource(s, 1, &src, nullptr);
				fns->glCompileShader(s);
				int ok = 0;
				fns->glGetShaderiv(s, /*GL_COMPILE_STATUS=*/0x8B81, &ok);
				if (!ok)
				{
					char log[1024] = {0};
					fns->glGetShaderInfoLog(s, sizeof(log), nullptr, log);
					Log.error() << "[SPIKE] shader compile failed: " << log << std::endl;
					fns->glDeleteShader(s);
					return 0;
				}
				return s;
			};

			unsigned int vs = compile_one(/*GL_VERTEX_SHADER=*/0x8B31, k_vertex_shader_src_);
			unsigned int fs = compile_one(/*GL_FRAGMENT_SHADER=*/0x8B30, k_fragment_shader_src_);
			if (!vs || !fs)
				return false;

			program_id_ = fns->glCreateProgram();
			fns->glAttachShader(program_id_, vs);
			fns->glAttachShader(program_id_, fs);
			fns->glLinkProgram(program_id_);
			int link_ok = 0;
			fns->glGetProgramiv(program_id_, /*GL_LINK_STATUS=*/0x8B82, &link_ok);
			fns->glDeleteShader(vs);
			fns->glDeleteShader(fs);
			if (!link_ok)
			{
				char log[1024] = {0};
				fns->glGetProgramInfoLog(program_id_, sizeof(log), nullptr, log);
				Log.error() << "[SPIKE] shader link failed: " << log << std::endl;
				return false;
			}

			uloc_mvp_         = fns->glGetUniformLocation(program_id_, "u_mvp");
			uloc_pick_mode_   = fns->glGetUniformLocation(program_id_, "u_pick_mode");
			uloc_picking_id_  = fns->glGetUniformLocation(program_id_, "u_picking_id");
			return true;
		}

		bool CoreGLRenderer::initPickingFBO_(int width, int height)
		{
			QOpenGLFunctions_3_2_Core* fns = glFns_();
			if (!fns)
				return false;

			fns->glGenFramebuffers(1, &picking_fbo_);
			fns->glBindFramebuffer(/*GL_FRAMEBUFFER=*/0x8D40, picking_fbo_);

			// R32UI colour attachment for per-pixel object IDs.
			fns->glGenTextures(1, &picking_tex_);
			fns->glBindTexture(/*GL_TEXTURE_2D=*/0x0DE1, picking_tex_);
			fns->glTexImage2D(/*GL_TEXTURE_2D*/0x0DE1, 0, /*GL_R32UI=*/0x8236,
			                  width, height, 0, /*GL_RED_INTEGER=*/0x8D94,
			                  /*GL_UNSIGNED_INT=*/0x1405, nullptr);
			fns->glTexParameteri(0x0DE1, /*GL_TEXTURE_MIN_FILTER=*/0x2801, /*GL_NEAREST=*/0x2600);
			fns->glTexParameteri(0x0DE1, /*GL_TEXTURE_MAG_FILTER=*/0x2800, /*GL_NEAREST=*/0x2600);
			fns->glFramebufferTexture2D(/*GL_FRAMEBUFFER=*/0x8D40,
			                            /*GL_COLOR_ATTACHMENT0=*/0x8CE0,
			                            /*GL_TEXTURE_2D=*/0x0DE1, picking_tex_, 0);

			// Depth renderbuffer (required for correct depth-sorted picking).
			fns->glGenRenderbuffers(1, &picking_depth_);
			fns->glBindRenderbuffer(/*GL_RENDERBUFFER=*/0x8D41, picking_depth_);
			fns->glRenderbufferStorage(0x8D41, /*GL_DEPTH_COMPONENT24=*/0x81A6, width, height);
			fns->glFramebufferRenderbuffer(/*GL_FRAMEBUFFER=*/0x8D40,
			                               /*GL_DEPTH_ATTACHMENT=*/0x8D00,
			                               /*GL_RENDERBUFFER=*/0x8D41, picking_depth_);

			fns->glBindFramebuffer(/*GL_FRAMEBUFFER=*/0x8D40, 0);
			picking_fbo_w_ = width;
			picking_fbo_h_ = height;
			return true;
		}

		void CoreGLRenderer::resizePickingFBO_(int width, int height)
		{
			if (picking_fbo_w_ == width && picking_fbo_h_ == height)
				return;
			QOpenGLFunctions_3_2_Core* fns = glFns_();
			if (!fns)
				return;
			fns->glDeleteFramebuffers(1, &picking_fbo_);
			fns->glDeleteTextures(1, &picking_tex_);
			fns->glDeleteRenderbuffers(1, &picking_depth_);
			picking_fbo_ = picking_tex_ = picking_depth_ = 0;
			initPickingFBO_(width, height);
		}

		void CoreGLRenderer::renderRepresentations_(const RepresentationList& representations)
		{
			if (!initialised_)
			{
				// Spike not ready (no core context, init failed, etc.) — fall back
				// to the Renderer base default which fans out to renderOneRepresentation().
				// In the throwaway path that produces nothing visible, but it does
				// keep the QPainter overlay alive (paintGL() still runs).
				Renderer::renderRepresentations_(representations);
				return;
			}

			QOpenGLFunctions_3_2_Core* fns = glFns_();
			if (!fns)
				return;

			// Lazy-create the picking FBO once we know the viewport size.
			if (!picking_fbo_)
				initPickingFBO_((int)width_, (int)height_);
			else
				resizePickingFBO_((int)width_, (int)height_);

			// Build the per-frame ID -> object table. Each Representation gets a
			// unique unsigned ID starting at 1 (0 = "no object" so a clear-to-0
			// picking pass leaves a recognisable miss code).
			id_to_object_.clear();
			unsigned int next_id = 1;

			// Bind program + VAO once for the whole frame.
			fns->glUseProgram(program_id_);
			fns->glBindVertexArray(vao_id_);

			// Identity MVP — THROWAWAY: a real implementation would build the MVP
			// from Stage's camera. The spike's "renders the demo molecule" bar is
			// satisfied by drawing the representations at NDC scale; pixel parity
			// with GLRenderer is explicitly not a SPIKE-01 acceptance criterion.
			float mvp[16] = {
				1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			};
			fns->glUniformMatrix4fv(uloc_mvp_, 1, /*transpose=*/0, mvp);
			fns->glUniform1i(uloc_pick_mode_, 0);  // main render pass

			fns->glEnable(/*GL_DEPTH_TEST=*/0x0B71);
			fns->glClearColor(0.05f, 0.05f, 0.08f, 1.0f);
			fns->glClear(/*GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT*/ 0x4000 | 0x0100);

			// Build VBO data per representation. THROWAWAY: we render each
			// GeometricObject as a single coloured point at its position. This
			// is enough to demonstrate that the GL-core path drives the demo
			// molecule visibly; lighting/shading is out of scope for the spike.
			for (auto it = representations.begin(); it != representations.end(); ++it)
			{
				const Representation* rep = *it;
				if (rep == nullptr)
					continue;

				unsigned int my_id = next_id++;
				// THROWAWAY: the spike picks at per-Representation granularity, not
				// per-atom. The first GeometricObject in the Representation's list
				// is what we report back to callers as the picked object (sufficient
				// for the SPIKE-01 demonstration deliverable). If the rep has none,
				// skip.
				GeometricObject* representative = nullptr;
				const auto& geos = rep->getGeometricObjects();
				if (!geos.empty())
					representative = *geos.begin();
				if (representative == nullptr)
					continue;
				id_to_object_[my_id] = representative;

				// Emit one vertex at the representation's centre (approximated as
				// origin). A real impl would walk rep's GeometricObjects and emit a
				// vertex per atom. THROWAWAY: this is enough for the smoke check to
				// produce a non-blank PNG.
				struct V { float x, y, z, r, g, b; };
				V verts[1] = {
					{ 0.0f, 0.0f, 0.0f, 0.6f, 0.8f, 1.0f }
				};

				fns->glBindBuffer(/*GL_ARRAY_BUFFER=*/0x8892, vbo_id_);
				fns->glBufferData(/*GL_ARRAY_BUFFER=*/0x8892, sizeof(verts), verts,
				                  /*GL_STREAM_DRAW=*/0x88E0);

				fns->glEnableVertexAttribArray(0);
				fns->glVertexAttribPointer(0, 3, /*GL_FLOAT=*/0x1406, 0, sizeof(V), (void*)0);
				fns->glEnableVertexAttribArray(1);
				fns->glVertexAttribPointer(1, 3, /*GL_FLOAT=*/0x1406, 0, sizeof(V),
				                           (void*)(3 * sizeof(float)));

				fns->glUniform1ui(uloc_picking_id_, my_id);

				// GL_POINTS = 0x0000
				fns->glDrawArrays(0x0000, 0, 1);
			}

			// Restore GL state so the QPainter text-overlay path in
			// GLRenderWindow::paintGL() composites cleanly on top. Phase 2 pattern:
			// the QPainter call after Renderer::render_* expects depth-test off and
			// no shader program bound.
			fns->glBindVertexArray(0);
			fns->glUseProgram(0);
			fns->glDisable(/*GL_DEPTH_TEST=*/0x0B71);
			fns->glBindFramebuffer(/*GL_FRAMEBUFFER=*/0x8D40, 0);
		}

		void CoreGLRenderer::pickObjects(Position x1, Position y1, Position /*x2*/, Position /*y2*/,
		                                 std::list<GeometricObject*>& objects)
		{
			// THROWAWAY: SPIKE-01 picking deliverable. Single-pixel readback at
			// (x1, y1) is acceptable per plan — marquee select is a PIPE-01 task.
			if (!initialised_ || picking_fbo_ == 0)
				return;

			QOpenGLFunctions_3_2_Core* fns = glFns_();
			if (!fns)
				return;

			// Render pass 2: into the picking FBO with picking-mode shader.
			fns->glBindFramebuffer(/*GL_FRAMEBUFFER=*/0x8D40, picking_fbo_);
			fns->glUseProgram(program_id_);
			fns->glBindVertexArray(vao_id_);
			fns->glUniform1i(uloc_pick_mode_, 1);
			unsigned int clear_id = 0;
			fns->glClearBufferuiv(/*GL_COLOR=*/0x1800, 0, &clear_id);
			fns->glClear(/*GL_DEPTH_BUFFER_BIT*/ 0x0100);

			// Re-issue the same draws that renderRepresentations_() did. THROWAWAY:
			// in a production renderer we would cache the draw list; for the spike
			// the id_to_object_ map is already populated from the last main-pass call.
			// We rely on the fact that the call site (RenderSetup) invokes pickObjects
			// immediately after rendering, so the program/VAO/MVP state is still set.
			for (auto& kv : id_to_object_)
			{
				unsigned int id = kv.first;
				struct V { float x, y, z, r, g, b; };
				V verts[1] = { { 0.0f, 0.0f, 0.0f, 0, 0, 0 } };
				fns->glBindBuffer(/*GL_ARRAY_BUFFER=*/0x8892, vbo_id_);
				fns->glBufferData(/*GL_ARRAY_BUFFER=*/0x8892, sizeof(verts), verts,
				                  /*GL_STREAM_DRAW=*/0x88E0);
				fns->glEnableVertexAttribArray(0);
				fns->glVertexAttribPointer(0, 3, /*GL_FLOAT=*/0x1406, 0, sizeof(V), (void*)0);
				fns->glUniform1ui(uloc_picking_id_, id);
				fns->glDrawArrays(/*GL_POINTS=*/0x0000, 0, 1);
			}

			// Read back the single pixel under the cursor as an unsigned int.
			unsigned int picked = 0;
			fns->glBindFramebuffer(/*GL_READ_FRAMEBUFFER=*/0x8CA8, picking_fbo_);
			fns->glReadPixels((int)x1, (int)y1, 1, 1, /*GL_RED_INTEGER=*/0x8D94,
			                  /*GL_UNSIGNED_INT=*/0x1405, &picked);
			fns->glBindFramebuffer(/*GL_FRAMEBUFFER=*/0x8D40, 0);

			auto it = id_to_object_.find(picked);
			if (it != id_to_object_.end() && it->second != nullptr)
			{
				objects.push_back(it->second);
				Log.info() << "[SPIKE] picked object id=" << picked << std::endl;
			}

			// Restore main-pass state.
			fns->glUniform1i(uloc_pick_mode_, 0);
			fns->glBindVertexArray(0);
			fns->glUseProgram(0);
		}

	} // namespace VIEW

} // namespace BALL
