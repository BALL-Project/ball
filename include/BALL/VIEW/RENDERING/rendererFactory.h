// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_RENDERING_RENDERERFACTORY_H
#define BALL_VIEW_RENDERING_RENDERERFACTORY_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

class QWidget;

namespace BALL
{
	namespace VIEW
	{
		class Renderer;
		class RenderSurface;

		/** Constructs renderers and surfaces by Kind so Scene never names a
		 *  concrete backend type.
		 *
		 *  This is the ARCH-02 construction switchboard: every \c new GLRenderer /
		 *  \c new GLRenderWindow for the primary (non-deferred) rendering paths is
		 *  centralised here. The Phase 5 backend swap becomes "one new \c case
		 *  plus two new files" -- \c scene.C is not re-touched.
		 *
		 *  It is intentionally stateless: a plain construction switchboard, no
		 *  singletons, no caching.
		 */
		namespace RendererFactory
		{
			/// The renderer/surface backends Scene can construct by enum.
			enum class Kind
			{
				OpenGL_Fixed,   ///< today's GLRenderer (fixed-function compat profile)
				Raytracer,      ///< CPU raytracer (BALL_HAS_RTFACT)
				POV,            ///< POV-Ray file renderer
				STL             ///< STL file renderer
				// VRML removed v1.7 (Phase 999.37): VRMLRenderer class was
				// BALL_DEPRECATED and unused in the BALLView UI (the menu item
				// labeled "Export VRML" actually invoked the STL renderer; the
				// surface was renamed to Export STL in this phase). No on-disk
				// .wrl format anywhere in the codebase consumed VRMLRenderer.
#ifdef BALL_SPIKE_BACKEND_GLCORE
				,
				/// THROWAWAY: GL 3.2+ core-profile renderer spike (Phase 5 / Plan 05-05).
				/// Only present when configured with -DBALL_SPIKE_BACKEND=GLCore.
				/// Not for production; removed before PIPE-01.
				OpenGL_Core
#endif
#ifdef BALL_SPIKE_BACKEND_QRHI
				,
				/// THROWAWAY: QRhiWidget-based renderer spike (Phase 5 / Plan 05-06).
				/// Requires Qt 6.7+ (QRhiWidget). Only present when configured with
				/// -DBALL_SPIKE_BACKEND=QRhi. Not for production; removed before PIPE-01.
				QRhi
#endif
			};

			/// Construct a Renderer for the given Kind. Caller owns the result.
			/// Returns 0 for an unhandled Kind.
			BALL_VIEW_EXPORT Renderer* makeRenderer(Kind kind);

			/// Construct an on-screen RenderSurface for the given Kind. Caller
			/// owns the result. \p parent is forwarded to the underlying QWidget.
			/// Returns 0 for Kinds that have no on-screen surface (file renderers)
			/// or an unhandled Kind.
			BALL_VIEW_EXPORT RenderSurface* makeSurface(Kind kind, QWidget* parent);
		}

	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_RENDERING_RENDERERFACTORY_H
