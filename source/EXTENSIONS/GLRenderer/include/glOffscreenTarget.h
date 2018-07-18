#ifndef BALL_GLRENDERER_GLOFFSCREENTARGET_H
#define BALL_GLRENDERER_GLOFFSCREENTARGET_H

#include <BALL/COMMON/global.h>
#include <BALL/VIEW/RENDERING/glRenderWindow.h>

#include <QtGui/QPaintDevice>

#include <boost/shared_ptr.hpp>

class QGLPixelBuffer;

namespace BALL
{
	namespace GLRenderer
	{				
		/** Model of the \link RenderWindow \endlink which uses OpenGL to render its buffer into a file.
		 * 
		 *  The class will try to use a pixel buffer target if this is available. If the OpenGL driver has
		 *  no support for PixelBufferObjects, we try to render into a window on screen instead.
		 */
		class BALL_VIEW_EXPORT GLOffscreenTarget :
			public BALL::VIEW::RenderWindow,
			public QPaintDevice
		{
			public:
				/** Create a new GLOffscreenTarget with context shared from an existing GLRenderWindow.
				 */
				GLOffscreenTarget(BALL::VIEW::GLRenderWindow* share_from, const String& filename);

				virtual void prepareRendering();
				virtual void prepareUpscaling(Size final_width, Size final_height);

				virtual bool resize(const unsigned int width, const unsigned int height);
				virtual void refresh();

				void tryUsePixelBuffer(bool use_pbo = true);

				QImage getImage();
				void updateImageTile(Size x_lower, Size y_lower, Size x_upper, Size y_upper);

				virtual QPaintEngine* paintEngine() const;
				virtual int metric(PaintDeviceMetric metric) const;

			protected:
				String filename_;

				BALL::VIEW::GLRenderWindow* share_from_;

				boost::shared_ptr<QGLPixelBuffer> pixel_buffer_;

				bool use_pixel_buffer_;

				QImage current_image_;
		};

	}
}
#endif // BALL_GLRENDERER_GLOFFSCREENTARGET_H
