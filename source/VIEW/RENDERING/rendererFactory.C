// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/RENDERING/rendererFactory.h>

#include <BALL/VIEW/RENDERING/RENDERERS/glRenderer.h>
#include <BALL/VIEW/RENDERING/RENDERERS/POVRenderer.h>
#include <BALL/VIEW/RENDERING/RENDERERS/STLRenderer.h>
#include <BALL/VIEW/RENDERING/glRenderWindow.h>

#ifdef BALL_HAS_RTFACT
# include <BALL/VIEW/RENDERING/RENDERERS/rtfactRenderer.h>
#endif

#ifdef BALL_SPIKE_BACKEND_GLCORE
// THROWAWAY SPIKE — Phase 5 prototype only (Plan 05-05).
# include <BALL/VIEW/RENDERING/RENDERERS/coreGLRenderer.h>
# include <cstdlib> // std::getenv for runtime opt-in gate
#endif

#ifdef BALL_SPIKE_BACKEND_QRHI
// THROWAWAY SPIKE — Phase 5 prototype only (Plan 05-06).
# include <BALL/VIEW/RENDERING/RENDERERS/rhiRenderer.h>
# include <BALL/VIEW/RENDERING/qtRhiSurface.h>
# ifndef BALL_SPIKE_BACKEND_GLCORE
#  include <cstdlib> // std::getenv for runtime opt-in gate (shared with GLCore arm)
# endif
#endif

namespace BALL
{
	namespace VIEW
	{
		namespace RendererFactory
		{

			Renderer* makeRenderer(Kind kind)
			{
#if defined(BALL_SPIKE_BACKEND_GLCORE) || defined(BALL_SPIKE_BACKEND_QRHI)
				// THROWAWAY SPIKE — runtime opt-in gate. The spike Kind(s) exist
				// at compile time (Kind::OpenGL_Core / Kind::QRhi in scope), but
				// production callers still pass Kind::OpenGL_Fixed because scene.C
				// is unchanged (Phase 02.1 boundary preserved). Intercept the
				// production Kind only when BALLVIEW_USE_SPIKE_BACKEND=1 is set
				// at launch; otherwise the original GLRenderer path is taken so
				// the spike build still runs the default renderer when the user
				// does not opt in. If both spike backends are compiled in (an
				// unusual configuration), QRhi wins.
				//
				// Strict "==1" parse (review WR-01): std::getenv returns non-null
				// for any value including "0", "false", "off". Treat only the
				// literal single character "1" as truthy so
				// BALLVIEW_USE_SPIKE_BACKEND=0 disables the spike as the comment
				// claims.
				const char* spike_env_ = std::getenv("BALLVIEW_USE_SPIKE_BACKEND");
				const bool spike_env_truthy_ =
					(spike_env_ != nullptr && spike_env_[0] == '1' && spike_env_[1] == '\0');
				if (kind == Kind::OpenGL_Fixed && spike_env_truthy_)
				{
#  if defined(BALL_SPIKE_BACKEND_QRHI)
					return new QRhiRenderer;
#  else
					return new CoreGLRenderer;
#  endif
				}
#endif
				switch (kind)
				{
					case Kind::OpenGL_Fixed:
						return new GLRenderer;

#ifdef BALL_SPIKE_BACKEND_GLCORE
					case Kind::OpenGL_Core:
						// THROWAWAY SPIKE — Plan 05-05.
						return new CoreGLRenderer;
#endif

#ifdef BALL_SPIKE_BACKEND_QRHI
					case Kind::QRhi:
						// THROWAWAY SPIKE — Plan 05-06.
						return new QRhiRenderer;
#endif

					case Kind::Raytracer:
#ifdef BALL_HAS_RTFACT
						return new RTfactRenderer();
#else
						// No raytracer backend in this build -- fall back to the
						// fixed-function GL renderer, matching what scene.C's
						// registerRenderers_() #else branch does today.
						return new GLRenderer;
#endif

					case Kind::POV:
						return new POVRenderer;

					case Kind::STL:
						return new STLRenderer;

					default:
						return 0;
				}
			}

			RenderSurface* makeSurface(Kind kind, QWidget* parent)
			{
				switch (kind)
				{
					case Kind::OpenGL_Fixed:
						return new GLRenderWindow(parent);

					case Kind::Raytracer:
						// The raytracer renders into a CPU buffer presented by a
						// GLRenderWindow, exactly as scene.C does today.
						return new GLRenderWindow(parent);

#ifdef BALL_SPIKE_BACKEND_GLCORE
					case Kind::OpenGL_Core:
						// THROWAWAY SPIKE — Plan 05-05. The spike reuses the existing
						// QOpenGLWidget surface; the QSurfaceFormat handed to
						// GLRenderWindow::gl_format_ remains the production compat
						// profile. The spike renderer detects (init()) whether the
						// granted context is core-profile and degrades to a no-op
						// otherwise -- it never crashes a regular run.
						return new GLRenderWindow(parent);
#endif

#ifdef BALL_SPIKE_BACKEND_QRHI
					case Kind::QRhi:
						// THROWAWAY SPIKE — Plan 05-06. QRhiWidget IS the surface
						// (multi-inheritance mirrors GLRenderWindow's RenderWindow +
						// QOpenGLWidget pattern). The QRhi backend is auto-selected
						// by Qt at runtime (Metal on macOS, D3D11/12 on Windows,
						// Vulkan or GL on Linux).
						return new QtRhiSurface(parent);
#endif

					case Kind::POV:
					case Kind::STL:
						// File renderers have no on-screen surface -- scene.C does
						// not construct a window for them. Keep parity.
						return 0;

					default:
						return 0;
				}
			}

		} // namespace RendererFactory

	} // namespace VIEW

} // namespace BALL
