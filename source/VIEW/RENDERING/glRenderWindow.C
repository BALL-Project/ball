// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/RENDERING/glRenderWindow.h>
#include <BALL/VIEW/WIDGETS/scene.h>
#include <BALL/COMMON/logStream.h>
#include <BALL/VIEW/KERNEL/common.h>
#include <BALL/VIEW/KERNEL/mainControl.h>

#include <QtCore/QEvent>
#include <QtGui/QPaintEvent>
#include <QtGui/QWindow>

//#define USE_GLPAINTPIXELS
#undef USE_GLPAINTPIXELS

namespace BALL 
{
	namespace VIEW 
	{
	  QGLFormat GLRenderWindow::gl_format_(
				QGL::DepthBuffer 		 | 
#ifndef BALL_OS_DARWIN
				QGL::StereoBuffers 	 | 
#endif
				QGL::DoubleBuffer 	 | 
				QGL::DirectRendering |
				QGL::SampleBuffers   |
				QGL::StencilBuffer);


		GLRenderWindow::GLRenderWindow()
			: QGLWidget(gl_format_),
			  stereo_delta_(0.),
			  m_screenTexID(0),
			  FB_TEXTURE_TARGET(GL_TEXTURE_2D),
			  FB_TEXTURE_FORMAT(GL_RGB),
			  FB_INTERNAL_TEXTURE_FORMAT(GL_RGB),
			  FB_TEXTURE_DATATYPE(GL_FLOAT),
			  ignore_events_(false),
			  down_sampling_factor_(1.)
		{		
			// we will swap buffers manually in the scene for synchronization
			setAutoBufferSwap(false);
			setAutoFillBackground(false);
		}

		GLRenderWindow::GLRenderWindow(QWidget* parent_widget, const char* /*name*/, Qt::WindowFlags w_flags)
			: QGLWidget(gl_format_, parent_widget, (QGLWidget*)0, w_flags),
			  stereo_delta_(0.),
			  m_screenTexID(0),
			  FB_TEXTURE_TARGET(GL_TEXTURE_2D),
			  FB_TEXTURE_FORMAT(GL_RGB),
			  FB_INTERNAL_TEXTURE_FORMAT(GL_RGB),
			  FB_TEXTURE_DATATYPE(GL_FLOAT),
			  ignore_events_(false),
			  down_sampling_factor_(1)
		{
			if (!QGLWidget::isValid())
			{
				Log.error() << "QGLWidget is not valid in Scene!" << std::endl;
			}
			// we will swap buffers manually in the scene for synchronization
			setAutoBufferSwap(false);
			setAutoFillBackground(false);
		}

		GLRenderWindow::GLRenderWindow(const GLRenderWindow& window, QWidget* parent_widget, const char* /*name*/, Qt::WindowFlags w_flags)
			: QGLWidget(gl_format_, parent_widget, &window, w_flags),
			  stereo_delta_(0.),
			  m_screenTexID(0),
			  FB_TEXTURE_TARGET(GL_TEXTURE_2D),
			  FB_TEXTURE_FORMAT(GL_RGB),
			  FB_INTERNAL_TEXTURE_FORMAT(GL_RGB),
			  FB_TEXTURE_DATATYPE(GL_FLOAT),
			  ignore_events_(false),
			  down_sampling_factor_(1.)
		{
			// we will swap buffers manually in the scene for synchronization
			setAutoBufferSwap(false);
			setAutoFillBackground(false);
		}

		GLRenderWindow::~GLRenderWindow()
		{
			deleteTexture();
		}

		bool GLRenderWindow::init()
		{
			checkGL();

			// TODO: is this necessary?
			if (!format().rgba())
				Log.error() << "No RGBA mode for OpenGl available." << std::endl;

			RenderWindow::init();
			bool result = false;

			FB_TEXTURE_TARGET = GL_TEXTURE_2D;

			if(m_fmt.getPixelFormat() == PixelFormat::RGBF_96)
			{
				FB_INTERNAL_TEXTURE_FORMAT = GL_RGB;
				FB_TEXTURE_FORMAT = GL_RGB;
				FB_TEXTURE_DATATYPE = GL_FLOAT;
				result = true;
			}
			else
			{
				if(m_fmt.getPixelFormat() == PixelFormat::RGBA_32)
				{
					FB_INTERNAL_TEXTURE_FORMAT = GL_RGBA;
					FB_TEXTURE_FORMAT = GL_RGBA;
					FB_TEXTURE_DATATYPE = GL_UNSIGNED_BYTE;
				}
				result = true;
			}			
			return result;
		}
		
		bool GLRenderWindow::resize(const unsigned int width, const unsigned int height)
		{						
			if(!RenderWindow::resize((int)ceil(width/down_sampling_factor_), (int)ceil(height/down_sampling_factor_)))
			{
				return false;
			}
			createTexture((int)ceil(width/down_sampling_factor_), (int)ceil(height/down_sampling_factor_));

			QGLWidget::resize(width, height);

			return true;
		}						

		void GLRenderWindow::refresh()
		{			
			RenderWindow::refresh();

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			glPushAttrib(GL_TEXTURE_BIT);
			glPushAttrib(GL_DEPTH_BUFFER_BIT);

#ifdef USE_GLPAINTPIXELS
			glDrawPixels(m_fmt.getWidth(), m_fmt.getHeight(), FB_TEXTURE_FORMAT, FB_TEXTURE_DATATYPE, m_pixels.get());
#else
			glBindTexture(FB_TEXTURE_TARGET, m_screenTexID);
			glTexSubImage2D(FB_TEXTURE_TARGET, 0, 0, 0, m_fmt.getWidth(), m_fmt.getHeight(), 
					FB_TEXTURE_FORMAT, FB_TEXTURE_DATATYPE, m_pixels.get());                

			glEnable(FB_TEXTURE_TARGET);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
#endif

			glDisable(GL_DEPTH_TEST);
			glMatrixMode(GL_PROJECTION);
			glPushMatrix();

			glLoadIdentity();

			glPushAttrib(GL_VIEWPORT_BIT);
#ifndef USE_GLPAINTPIXELS
			glViewport(0, 0, down_sampling_factor_*m_fmt.getWidth(), down_sampling_factor_*m_fmt.getHeight());
			float aspectRatio = static_cast<float>(m_fmt.getWidth()) / m_fmt.getHeight();
			glOrtho(-aspectRatio, aspectRatio, -1.0f, 1.0f, -1.0f, 1.0f);
			
			
			float origWidth = static_cast<float>(m_fmt.getWidth());
			float newWidth = (origWidth + fabs(stereo_delta_));
			float newRatio = origWidth / newWidth;
			float deltaRatio = 1. - newRatio;
				
			glBegin(GL_QUADS);
			
			if (stereo_delta_ <= 0.)
			{
				glTexCoord2f(0.0f+deltaRatio, 0.0f);
				glVertex2f(-aspectRatio, -1.0f);
			
				glTexCoord2f(1.0f, 0.0f );
				glVertex2f(aspectRatio, -1.0f);	

				glTexCoord2f(1.0f, 1.0f );
				glVertex2f(aspectRatio, 1.0f);	

				glTexCoord2f(0.0f+deltaRatio, 1.0f);
				glVertex2f(-aspectRatio, 1.0f);
			}
			else
			{
				glTexCoord2f(0.0f, 0.0f);
				glVertex2f(-aspectRatio, -1.0f);
			
				glTexCoord2f(1.0f-deltaRatio, 0.0f );
				glVertex2f(aspectRatio, -1.0f);	

				glTexCoord2f(1.0f-deltaRatio, 1.0f );
				glVertex2f(aspectRatio, 1.0f);	

				glTexCoord2f(0.0f, 1.0f);
				glVertex2f(-aspectRatio, 1.0f);
			}


			glEnd();
#endif
			glPopAttrib();

			glPopMatrix();	
			glMatrixMode(GL_MODELVIEW);				

			glPopAttrib();
			glPopAttrib();
		}

		void GLRenderWindow::renderText(int x, int y, const String& text, const ColorRGBA& color, Size size)
		{
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();

			glViewport(0, 0, m_fmt.getWidth(), m_fmt.getHeight());

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			QFont font;
			font.setPixelSize(size);
			font.setBold(true);

			glDisable(GL_LIGHTING);
			glColor4ub(color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());
			QGLWidget::renderText(x, y, text.c_str(), font);
			glEnable(GL_LIGHTING);
		}

		void GLRenderWindow::renderText(float x, float y, float z, const String& text, const ColorRGBA& color, Size size)
		{
			// TEST!
			return;
			QFont font;
			font.setPixelSize(size);
			font.setBold(true);

			glDisable(GL_LIGHTING);
			glColor4ub(color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());
			QGLWidget::renderText(x, y, z, text.c_str(), font);
			glEnable(GL_LIGHTING);
		}

		void GLRenderWindow::createTexture(const unsigned int width, const unsigned int height)
		{
			if(m_screenTexID != 0)
			{
				deleteTexture();
			}

			glGenTextures(1, &m_screenTexID);

			glBindTexture(FB_TEXTURE_TARGET, m_screenTexID);			                

			glTexParameteri(FB_TEXTURE_TARGET, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(FB_TEXTURE_TARGET, GL_TEXTURE_MAG_FILTER, GL_LINEAR);                
					
			glTexImage2D(FB_TEXTURE_TARGET, 0, FB_INTERNAL_TEXTURE_FORMAT, width, height, 0, FB_TEXTURE_FORMAT, FB_TEXTURE_DATATYPE, NULL);                                

			glBindTexture(FB_TEXTURE_TARGET, 0);
		}

		void GLRenderWindow::deleteTexture()
		{
			glDeleteTextures(1, &m_screenTexID);
			m_screenTexID = 0;
		}

		bool GLRenderWindow::errorInGL(GLenum& error)
		{
				error = glGetError();
				return (error != GL_NO_ERROR);
		}

		String GLRenderWindow::getGLErrorString(GLenum error)
		{
				String result;
				switch(error)
				{
				case GL_INVALID_ENUM:
						result = "Invalid enumeration value";
						break;
				case GL_INVALID_VALUE:
						result = "Numeric argument out of range";
						break;
				case GL_INVALID_OPERATION:
						result = "Operation illegal in current state";
						break;
				case GL_STACK_OVERFLOW:
						result = "Command would cause stack overflow";
						break;
				case GL_STACK_UNDERFLOW:
						result = "Command would cause stack underflow";
						break;
				case GL_OUT_OF_MEMORY:
						result = "Not enough memory left to execute command";
						break;
				case GL_TABLE_TOO_LARGE:
						result = "The specified table is too large";
						break;
				default:
						result = "Uknown OpenGL error";
				}
				return result;
		}

		void GLRenderWindow::checkGL()
		{
				GLenum err;
				if(errorInGL(err))
				{
					BALL::Log.error() << "Error in OpenGL: " << getGLErrorString(err) << std::endl;
				}
		}

		void GLRenderWindow::customEvent(QEvent* evt)
		{
			switch(static_cast<EventsIDs>(evt->type())) {
				case RENDER_TO_BUFFER_FINISHED_EVENT:
					refresh();
					safeBufferSwap();
					break;
			}
		}

		void GLRenderWindow::paintEvent(QPaintEvent* e)
		{
			if (!ignore_events_) 
			{
				QGLWidget::paintEvent(e);
			}
		}

		void GLRenderWindow::lockGLContext()
		{
			contex_mutex_.lock();
			makeCurrent();
		}

		void GLRenderWindow::unlockGLContext()
		{
			doneCurrent();
			contex_mutex_.unlock();
		}
		
		void GLRenderWindow::setupStereo(float eye_separation, float focal_length)
		{
				float aperture = 60.;
				float width = static_cast<float>(m_fmt.getWidth());
				
				//formula according to Paul Bourke
				//http://local.wasp.uwa.edu.au/~pbourke/miscellaneous/stereographics/stereorender/
				stereo_delta_ = (fabs(eye_separation) * width) / (focal_length * tan(Angle(aperture, false).toRadian())); 
				std::cout << stereo_delta_ << std::endl;
		}

		void GLRenderWindow::safeBufferSwap()
		{
			if(isVisible() && getMainControl()->windowHandle()->isExposed())
			{
				swapBuffers();
			}
		}
	} // namespace VIEW
} //namespace BALL

