// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/RENDERING/rendererFactory.h>

#include <BALL/VIEW/RENDERING/RENDERERS/glRenderer.h>
#include <BALL/VIEW/RENDERING/RENDERERS/POVRenderer.h>
#include <BALL/VIEW/RENDERING/RENDERERS/STLRenderer.h>
#include <BALL/VIEW/RENDERING/RENDERERS/VRMLRenderer.h>
#include <BALL/VIEW/RENDERING/glRenderWindow.h>

#ifdef BALL_HAS_RTFACT
# include <BALL/VIEW/RENDERING/RENDERERS/rtfactRenderer.h>
#endif

#ifdef BALL_SPIKE_BACKEND_GLCORE
// THROWAWAY SPIKE — Phase 5 prototype only (Plan 05-05).
# include <BALL/VIEW/RENDERING/RENDERERS/coreGLRenderer.h>
# include <cstdlib> // std::getenv for runtime opt-in gate
#endif

namespace BALL
{
	namespace VIEW
	{
		namespace RendererFactory
		{

			Renderer* makeRenderer(Kind kind)
			{
#ifdef BALL_SPIKE_BACKEND_GLCORE
				// THROWAWAY SPIKE — runtime opt-in gate. The spike Kind exists at
				// compile time (Kind::OpenGL_Core is in scope), but production
				// callers still pass Kind::OpenGL_Fixed because scene.C is unchanged
				// (Phase 02.1 boundary preserved). Intercept the production Kind
				// only when BALLVIEW_USE_SPIKE_BACKEND=1 is set at launch; otherwise
				// the original GLRenderer path is taken so the spike build still
				// runs the default renderer when the user does not opt in.
				if (kind == Kind::OpenGL_Fixed && std::getenv("BALLVIEW_USE_SPIKE_BACKEND") != nullptr)
				{
					return new CoreGLRenderer;
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

					case Kind::VRML:
						return new VRMLRenderer;

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

					case Kind::POV:
					case Kind::STL:
					case Kind::VRML:
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
