// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_RENDERING_RENDERERS_CUDARAYTRACINGRENDERER_H
#define BALL_VIEW_RENDERING_RENDERERS_CUDARAYTRACINGRENDERER_H

#define BALLVIEW_CUDAVOLUMERENDERER_THROW(exceptionName, message) (throw BALL::Exception::##exceptionName##(__FILE__, __LINE__, message))
#include <BALL/VIEW/RENDERING/RENDERERS/raytracingRenderer.h>
#include <cudamm/cuda.hpp>

namespace BALL
{
	namespace VIEW
	{
		class CudaVolumeRenderer
			: public RaytracingRenderer
		{			
			public:			

            /* RT renderer methods */
                virtual bool init(const Scene& scene) 
                    throw(BALL::Exception::CUDAError)				
            {
                try
	            {
		            cuda::Cuda cudaCtx(0);
		            cuda::Module mod("../libVIEW/GeneratedFiles/Debug/raytracer_debug.cubin");
                }
                catch(cuda::Exception const &e)
	            {
                    BALL::Log.error() << "Initialization of CudaVolumeRenderer failed" << std::endl;
                    BALLVIEW_CUDAVOLUMERENDERER_THROW(CUDAError, e.what());                    
	            }

                return true;
            }
						
			virtual String getRenderer()	
				throw()
            {
                return "CUDA Volume Renderer v1";
            }
           
			virtual void formatUpdated()
            {
            
            }
            			
			virtual void prepareBufferedRendering(const Stage& stage)
            {
            
            }
			
			virtual void renderToBufferImpl(FrameBufferPtr buffer)
            {               
                FrameBufferFormat fmt = buffer->getFormat();
                if(fmt.getPixelFormat() == PixelFormat::RGBA_32)
                {
                    renderImpl<int>(reinterpret_cast<int*>(buffer->getData()), fmt.getWidth(), fmt.getHeight());
                }               
                if(fmt.getPixelFormat() == PixelFormat::RGBF_96)
                {
                    renderImpl<float>(reinterpret_cast<float*>(buffer->getData()), fmt.getWidth(), fmt.getHeight());
                }
            }

        private:

            template<typename taPixelType>
            void renderImpl(taPixelType* buffer, const unsigned int width, const unsigned int height);

            template<>
            void renderImpl(int* buffer, const unsigned int width, const unsigned int height)
            {
                for (unsigned int j = 0; j < height; j++)
                {
                    for (unsigned int i = 0; i < width; i++)
                    {                        
                        if ( (((i / 64) % 2) == 0) && (((j / 64) % 2) == 0))
                        {                            
                            buffer[j*width + i] = (0 << 24) | (255 << 16) | (0 << 8) | (0);                            
                        }
                        else
                        {
                            buffer[j*width + i] = (0 << 24) | (0 << 16) | (0 << 8) | (255);                            
                        }

                    }
                } 
            }

            template<>
            void renderImpl(float* buffer, const unsigned int width, const unsigned int height)
            {
                for (unsigned int j = 0; j < height; j++)
                {
                    for (unsigned int i = 0; i < width; i++)
                    {                        
                        if ( (((i / 64) % 2) == 0) && (((j / 64) % 2) == 0))
                        {                                                        
                            buffer[3 * (j*width + i) + 0] = 1;
                            buffer[3 * (j*width + i) + 1] = 0;
                            buffer[3 * (j*width + i) + 2] = 0;                            
                        }
                        else
                        {                            
                            buffer[3 * (j*width + i) + 0] = 0;
                            buffer[3 * (j*width + i) + 1] = 1;
                            buffer[3 * (j*width + i) + 2] = 0;                            
                        }

                    }
                } 
            }

		};


	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_RENDERING_CUDARAYTRACINGRENDERER_H
