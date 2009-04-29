// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_RENDERING_RAYTRACINGRENDERER_H
#define BALL_VIEW_RENDERING_RAYTRACINGRENDERER_H

#ifndef BALL_VIEW_RENDERING_RENDERER_H
#	include <BALL/VIEW/RENDERING/renderer.h>
#endif

#ifndef BALL_VIEW_KERNEL_STAGE_H
# include <BALL/VIEW/KERNEL/stage.h>
#endif

#include <BALL/VIEW/RENDERING/bufferedRenderer.h>

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
			virtual void updateMaterialForRepresentation(Representation const* rep, const Stage::RaytracingMaterial& new_material) = 0;

        protected:

			/* BufferedRender protected methods */
			virtual bool supports(const FrameBufferFormat &format) const;
		};

	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_RENDERING_RAYTRACINGRENDERER_H
