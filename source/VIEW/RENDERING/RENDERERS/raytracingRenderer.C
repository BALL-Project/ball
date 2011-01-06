// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/RENDERING/RENDERERS/raytracingRenderer.h>
#include <BALL/VIEW/WIDGETS/scene.h>

namespace BALL
{
	namespace VIEW
	{		
		bool RaytracingRenderer::supports(const PixelFormat &format) const
		{
			return ((format == PixelFormat::RGBA_32) || (format == PixelFormat::RGBF_96));
		}

		Resolution RaytracingRenderer::getSupportedResolution(
			const Resolution &min, const Resolution &max,
			const PixelFormat &format) const
            throw(BALL::Exception::FormatUnsupported)
		{
			if(!supports(format))
			{
                throw BALL::Exception::FormatUnsupported(__FILE__, __LINE__);
			}
			return max;
		}		   
		
		bool RaytracingRenderer::supports(const FrameBufferFormat &format) const
		{
            return supports(format.getPixelFormat());				
		}

		std::vector<float> RaytracingRenderer::intersectRaysWithGeometry(const std::vector<Vector3>& origins,
		                                                                 const std::vector<Vector3>& directions)
		{
			Log.error() << "Sorry, intersectRaysWithGeometry() not implemented for this kind of renderer!" << std::endl;

			return std::vector<float>();
		}

	} // namespace VIEW

} // namespace BALL

