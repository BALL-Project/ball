// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_RENDERING_GLOFFSCREENTARGET_H
#define BALL_VIEW_RENDERING_GLOFFSCREENTARGET_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#ifndef BALL_VIEW_RENDERING_RENDERWINDOW_H
# include <BALL/VIEW/RENDERING/renderWindow.h>
#endif

#ifndef BALL_VIEW_RENDERING_GLRENDERWINDOW_H
# include <BALL/VIEW/RENDERING/glRenderWindow.h>
#endif


#include <QtGui/QPaintDevice>

#include <boost/shared_ptr.hpp>

class QGLPixelBuffer;

namespace BALL
{
	namespace VIEW
	{				
		/** Model of the \link RenderWindow \endlink which uses OpenGL to render its buffer into a file.
		 * 
		 *  The class will try to use a pixel buffer target if this is available. If the OpenGL driver has
		 *  no support for PixelBufferObjects, we try to render into a window on screen instead.
		 */
		class BALL_VIEW_EXPORT GLOffscreenTarget
			: public t_RenderWindow,
				public QPaintDevice
		{
			public:
				/** Create a new GLOffscreenTarget with context shared from an existing GLRenderWindow.
				 */
				GLOffscreenTarget(GLRenderWindow* share_from, const String& filename);

				virtual void prepareRendering();

				virtual bool resize(const unsigned int width, const unsigned int height);
				virtual void refresh();

				virtual QPaintEngine* paintEngine() const;

			protected:
				String     		 filename_;

				GLRenderWindow* share_from_;

				boost::shared_ptr<QGLPixelBuffer> pixel_buffer_;

				bool use_pixel_buffer_;
		};

	}
}
#endif // BALL_VIEW_RENDERING_GLOFFSCREENTARGET_H
