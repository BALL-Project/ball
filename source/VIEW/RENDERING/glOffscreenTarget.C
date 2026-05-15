// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/RENDERING/glOffscreenTarget.h>

#include <QtWidgets/QApplication>
#include <QtGui/QImage>
#include <QtGui/QPaintEvent>
#include <QtGui/QPainter>

// Qt 6: QOpenGLFramebufferObject moved from QtGui to the QtOpenGL module.
#include <QtOpenGL/QOpenGLFramebufferObject>

namespace BALL
{
	namespace VIEW
	{
		GLOffscreenTarget::GLOffscreenTarget(GLRenderWindow* share_from, const String& filename)
			: RenderWindow(),
				QPaintDevice(),
				filename_(filename),
				share_from_(share_from),
				fbo_(),
				current_image_(share_from->width(), share_from->height(), QImage::Format_ARGB32)
		{
		}

		void GLOffscreenTarget::prepareRendering()
		{
			// FBOs have no context of their own: make the shared GLRenderWindow
			// context current, then bind the FBO as the render target.
			if (share_from_)
				share_from_->makeCurrent();

			if (fbo_)
				fbo_->bind();
		}

		void GLOffscreenTarget::beginFrame()
		{
			// An FBO has no context of its own -- making the shared
			// GLRenderWindow context current and binding the FBO is exactly
			// what prepareRendering() already does.
			prepareRendering();
		}

		void GLOffscreenTarget::endFrame()
		{
			// Nothing to swap or submit for an offscreen FBO target.
		}

		void* GLOffscreenTarget::nativeHandle()
		{
			return static_cast<QPaintDevice*>(this);
		}

		bool GLOffscreenTarget::resize(const unsigned int width, const unsigned int height)
		{
			RenderWindow::resize(width, height);

			// Creating the FBO requires the shared context to be current.
			if (share_from_)
				share_from_->makeCurrent();

			fbo_ = boost::shared_ptr<QOpenGLFramebufferObject>(
				new QOpenGLFramebufferObject((int)width, (int)height,
				                             QOpenGLFramebufferObject::CombinedDepthStencil));
			return true;
		}

		void GLOffscreenTarget::tryUsePixelBuffer(bool /*use_pbo*/)
		{
			// No-op: the offscreen target is always FBO-backed now. The legacy
			// Qt4-era pixel-buffer class was removed with the GL widget port;
			// FBOs are universally available on the target GL versions.
		}

		QImage GLOffscreenTarget::getImage()
		{
			if (fbo_)
				return fbo_->toImage();
			else if (share_from_)
				return share_from_->grabFramebuffer();

			return QImage();
		}

		void GLOffscreenTarget::prepareUpscaling(Size final_width, Size final_height)
		{
			if (fbo_)
				current_image_ = QImage(final_width, final_height, fbo_->toImage().format());
			else if (share_from_)
				current_image_ = QImage(final_width, final_height, share_from_->grabFramebuffer().format());
			// Qt 6: QPainter::setRedirected / restoreRedirected were removed (the
			// painter-redirection API is gone). The tiling-render path paints into
			// current_image_ directly via QPainter on the QPaintDevice; the
			// legacy redirect call was a perf hint, not a correctness requirement.
			// THROWAWAY-context note: revisit during PIPE-01.
		}

		void GLOffscreenTarget::updateImageTile(Size x_lower, Size y_lower, Size x_upper, Size y_upper)
		{
			QImage current_screen;

			if (fbo_)
				current_screen = fbo_->toImage();
			else if (share_from_)
				current_screen = share_from_->grabFramebuffer();

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
