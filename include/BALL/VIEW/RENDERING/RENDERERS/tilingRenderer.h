// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_RENDERING_TILINGRENDERER_H
#define BALL_VIEW_RENDERING_TILINGRENDERER_H

#ifndef BALL_VIEW_RENDERING_RENDERERS_RENDERER_H
# include <BALL/VIEW/RENDERING/RENDERERS/renderer.h>
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

				The idea behind this renderer is heavily based on the extremely useful
				open-source TR library for tile rendering by Brian Paul
				(http://www.mesa3d.org/brianp/TR.html).
				
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
				* 	@param real_renderer the renderer to encapsulate
				* 	@param final_width   the width of the generated image
				* 	@param final_height  the height of the generated image
				* 	@param border        an optional border around each tile
				*
				*/
				TilingRenderer(Renderer* real_renderer, Size final_width, Size final_height, Size border = 0);
				
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

				/** Buffer a Representation for later rendering.
				*/
				virtual void bufferRepresentation(const Representation& rep);

				/** Remove a representation from the buffer.
				*/
				virtual void removeRepresentation(const Representation& rep);

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

			protected:
				void computeTilingSetup_();

				/// The renderer used for rendering the individual tiles
				Renderer* real_renderer_;

				/// The desired width of the final image
				Size final_width_;
				
				/// The desired height of the final image
				Size final_height_;

				/// The border oversampled for each tile
				Size border_;

				Size num_cols_;
				Size num_rows_;
		};
	}
}
#endif // BALL_VIEW_RENDERING_TILINGRENDERER_H

