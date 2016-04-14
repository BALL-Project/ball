// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/RENDERING/glOffscreenTarget.h>

#include <QtWidgets/QApplication>
#include <QtGui/QImage>
#include <QtGui/QPaintEvent>
#include <QtGui/QPainter>

#include <QtOpenGL/QGLPixelBuffer>

namespace BALL
{
	namespace VIEW
	{
		GLOffscreenTarget::GLOffscreenTarget(GLRenderWindow* share_from, const String& filename)
			: RenderWindow(),
				QPaintDevice(),
				filename_(filename),
				share_from_(share_from),
				pixel_buffer_(),
				use_pixel_buffer_(QGLPixelBuffer::hasOpenGLPbuffers()),
				current_image_(share_from->width(), share_from->height(), QImage::Format_ARGB32)
		{
		}

		void GLOffscreenTarget::prepareRendering()
		{
			if (use_pixel_buffer_ && pixel_buffer_)
				pixel_buffer_->makeCurrent();
			else if (share_from_)
				share_from_->makeCurrent();
		}

		bool GLOffscreenTarget::resize(const unsigned int width, const unsigned int height)
		{
			RenderWindow::resize(width, height);

			if (use_pixel_buffer_)
			{
				pixel_buffer_ = boost::shared_ptr<QGLPixelBuffer>(new QGLPixelBuffer(width, height, share_from_->format(), share_from_));
			}
			return true;
		}

		void GLOffscreenTarget::tryUsePixelBuffer(bool use_pbo)
		{
			use_pixel_buffer_ = QGLPixelBuffer::hasOpenGLPbuffers() && use_pbo;
		}

		QImage GLOffscreenTarget::getImage()
		{
			if (use_pixel_buffer_ && pixel_buffer_)
				return pixel_buffer_->toImage();
			else if (share_from_)
				return share_from_->grabFrameBuffer();

			return QImage();
		}

		void GLOffscreenTarget::prepareUpscaling(Size final_width, Size final_height)
		{
			if (use_pixel_buffer_ && pixel_buffer_)
				current_image_ = QImage(final_width, final_height, pixel_buffer_->toImage().format());
			else if (share_from_)
				current_image_ = QImage(final_width, final_height, share_from_->grabFrameBuffer().format());
			QPainter::setRedirected(this, &current_image_);
		}

		void GLOffscreenTarget::updateImageTile(Size x_lower, Size y_lower, Size x_upper, Size y_upper)
		{
			QImage current_screen;

			if (use_pixel_buffer_ && pixel_buffer_)
				current_screen = pixel_buffer_->toImage();
			else if (share_from_)
				current_screen = share_from_->grabFrameBuffer();

			if (!current_screen.isNull())
			{
				// compute the coordinates of the rectangle in the final image (remembering
				// that Qt sets y to 0 on the *upper* boundary of the image!)
				QPoint  top_left(x_lower, current_image_.height() - (y_upper+1));
				QPoint bot_right(x_upper, current_image_.height() - (y_lower+1));

				QPainter p(&current_image_);
				p.drawImage(QRect(top_left, bot_right), current_screen, current_screen.rect());
				p.end();
			}
		}
		
		void GLOffscreenTarget::refresh()
		{
			if (!current_image_.isNull())
				current_image_.save(filename_.c_str(), "PNG");
		}

		QPaintEngine* GLOffscreenTarget::paintEngine() const
		{
			return current_image_.paintEngine();
		}

		int GLOffscreenTarget::metric(PaintDeviceMetric m) const
		{
			// Unfortunately, metric() is protected in PaintDevice, so we
			// cannot simply redirect the call to current_image_.
			// Hence, some tedious conversions have to be made here

			int result = 0;

			switch (m)
			{
				case QPaintDevice::PdmWidth:  
					result = current_image_.width(); 
					break;
				case QPaintDevice::PdmHeight: 
					result = current_image_.height();
					break;
				case QPaintDevice::PdmWidthMM:
				  result = current_image_.widthMM(); 
					break;
				case QPaintDevice::PdmHeightMM: 
					result = current_image_.heightMM();
					break;
				case QPaintDevice::PdmNumColors: 
					result = current_image_.colorCount();
					break;
				case QPaintDevice::PdmDepth: 
					result = current_image_.depth();
					break;
				case QPaintDevice::PdmDpiX: 
					result = current_image_.logicalDpiX();
					break;
				case QPaintDevice::PdmDpiY: 
					result = current_image_.logicalDpiY();
					break;
				case QPaintDevice::PdmPhysicalDpiX: 
					result = current_image_.physicalDpiX();
					break;
				case QPaintDevice::PdmPhysicalDpiY: 
					result = current_image_.physicalDpiY();
					break;
				default:
					result = 0;
			}

			return result;
		}

	}
}
