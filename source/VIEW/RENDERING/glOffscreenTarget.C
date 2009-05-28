// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/RENDERING/glOffscreenTarget.h>

#include <QtGui/QApplication>
#include <QtGui/QImage>
#include <QtGui/QPaintEvent>
#include <QtGui/QPainter>

#include <QtOpenGL/QGLPixelBuffer>

namespace BALL
{
	namespace VIEW
	{
		GLOffscreenTarget::GLOffscreenTarget(GLRenderWindow* share_from, const String& filename)
			: t_RenderWindow(),
				QPaintDevice(),
				filename_(filename),
				share_from_(share_from),
				pixel_buffer_(),
				use_pixel_buffer_(QGLPixelBuffer::hasOpenGLPbuffers())
		{
		}

		void GLOffscreenTarget::prepareRendering()
		{
			if (use_pixel_buffer_ && pixel_buffer_)
				pixel_buffer_->makeCurrent();
		}

		bool GLOffscreenTarget::resize(const unsigned int width, const unsigned int height)
		{
			if (use_pixel_buffer_)
			{
				pixel_buffer_ = boost::shared_ptr<QGLPixelBuffer>(new QGLPixelBuffer(width, height, share_from_->format(), share_from_));
				QPainter::setRedirected(this, pixel_buffer_.get());
			}
		}

		void GLOffscreenTarget::refresh()
		{
			if (use_pixel_buffer_ && pixel_buffer_)
				pixel_buffer_->toImage().save(filename_.c_str(), "PNG");
		}

		QPaintEngine* GLOffscreenTarget::paintEngine() const
		{
			if (pixel_buffer_)
			{
				return pixel_buffer_->paintEngine();
			}
			else
				return NULL;
		}
	}
}
