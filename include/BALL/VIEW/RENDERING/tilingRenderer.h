// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_RENDERING_TILINGRENDERER_H
#define BALL_VIEW_RENDERING_TILINGRENDERER_H

#ifndef BALL_VIEW_RENDERING_RENDERER_H
# include <BALL/VIEW/RENDERING/renderer.h>
#endif

#ifndef BALL_VIEW_RENDERING_RENDERTARGET_H
# include <BALL/VIEW/RENDERING/renderTarget.h>
#endif

namespace BALL
{
	namespace VIEW
	{
		/** Offscreen rendering with arbitrary resolution.
				This class encapsulates a renderer object which it uses to render a
				number of individual tiles to compose them into a full image. This can
				be used to render very large images into files.
				\ingroup ViewRendering
		*/
		class BALL_VIEW_EXPORT TilingRenderer
			:	public Renderer
		{
			public:

				/**	@name	Constructors and Destructors
				*/	
				//@{

				/** Detailed Constructor.
				*
				* 	@param real_renderer the renderer to encapsulate.
				*/
				TilingRenderer(Renderer* real_renderer);
				
				/**	Copy constructor.
				*/
				TilingRenderer(const TilingRenderer& renderer);

				/** Destructor
				*/
				virtual ~TilingRenderer() {}

				/// Set the light sources according to the stage
				virtual void setLights(bool reset_all = false);

				/** Update the camera position either from a given Camera, or from the default Stage.
				 */
				virtual void updateCamera(const Camera* camera = 0);

				/// Update the background color from the stage
				virtual void updateBackgroundColor();

				///
				virtual bool finish();

				/** Compute the 3D position on the view plane corresponding
				 *  to point (x,y) on the view port
				 */
				virtual Vector3 mapViewportTo3D(Position x, Position y);

				/** Compute the 2D position on the screen corresponding
				 *  to the 3D point vec
				 */
				virtual Vector2 map3DToViewport(const Vector3& vec);

				//@}
				/**	@name	Accessors
				*/
				//@{
				
				/** Render a Representation.
				*/
				virtual bool renderOneRepresentation(const Representation& representation);

				/// Set the size of the display
				virtual void setSize(float width, float height);

				/** Render a ruler.
				 *
				 *  If supported by the renderer implementation, this function will produce
				 *  a simple ruler that is rendered together with the other representations.
				 *  The main use of this function is in the edit mode, where it can help to
				 *  straighten-up structures and to correctly estimate angles and distances.
				 */
				virtual void renderRuler();

				//@}
				/**	@name Predicates
				*/
				//@{
				
				///
				virtual void renderToBuffer(RenderTarget* target);

				/// Wrapper for the renderering of special GeometricObjects
				virtual void render_(const GeometricObject* object);

			protected:

				Renderer* real_renderer_;
		};
	}
}
#endif // BALL_VIEW_RENDERING_TILINGRENDERER_H

