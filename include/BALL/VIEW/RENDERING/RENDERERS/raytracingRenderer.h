// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_RENDERING_RENDERERS_RAYTRACINGRENDERER_H
#define BALL_VIEW_RENDERING_RENDERERS_RAYTRACINGRENDERER_H

#ifndef BALL_VIEW_RENDERING_RENDERERS_RENDERER_H
#	include <BALL/VIEW/RENDERING/RENDERERS/renderer.h>
#endif

#ifndef BALL_VIEW_KERNEL_STAGE_H
# include <BALL/VIEW/KERNEL/stage.h>
#endif

#ifndef BALL_VIEW_RENDERING_RENDERERS_BUFFEREDRENDERER_H
#include <BALL/VIEW/RENDERING/RENDERERS/bufferedRenderer.h>
#endif

#include <vector>

namespace BALL
{
	namespace VIEW
	{
		class Scene;

		/** RaytracingRenderer
		 		General interface for renderer, which provides raytracing instead of regular rendering.                
				\ingroup ViewRendering
		*/
		class BALL_VIEW_EXPORT RaytracingRenderer
			:	public BufferedRenderer				
		{			
		public:

			/* 
			 * \link BufferedRenderer \endlink public methods 
			 */
			virtual bool supports(const PixelFormat &format) const;			

			virtual Resolution getSupportedResolution(
				const Resolution &min, const Resolution &max,
				const PixelFormat &format) const
                throw(BALL::Exception::FormatUnsupported);			
					
			/*
			 * Get description of the renderer int textual form
			 */
			virtual String getRenderer() = 0;    

			virtual void updateMaterialForRepresentation(Representation const* rep) = 0;

			/** Raytracing-related functionality **/
			//@{ 
			/** Intersect a set of rays with the geometry buffered by this renderer.
			 *
			 *  This function will intersect the rays 
			 *
			 *     origins[i] + l * directions[i]
			 *
			 *  with the geometry that has been buffered by this renderer previously.
			 */
			virtual std::vector<float> intersectRaysWithGeometry(const std::vector<Vector3>& origins, 
			                                                     const std::vector<Vector3>& directions);

			//@}
		protected:

			/* BufferedRender protected methods */
			virtual bool supports(const FrameBufferFormat &format) const;
		};

	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_RENDERING_RAYTRACINGRENDERER_H
