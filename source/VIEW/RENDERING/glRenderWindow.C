// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/KERNEL/common.h>
#ifdef BALL_HAS_GLEW
#	include <GL/glew.h>
#endif

#include <BALL/VIEW/RENDERING/glRenderWindow.h>
#include <BALL/VIEW/RENDERING/renderSetup.h>
#include <BALL/VIEW/RENDERING/RENDERERS/glRenderer.h>
#include <BALL/VIEW/WIDGETS/scene.h>
#include <BALL/COMMON/logStream.h>
#include <BALL/VIEW/KERNEL/mainControl.h>

#include <QtCore/QEvent>
#include <QtGui/QWindow>
#include <QtGui/QOpenGLContext>
#include <QtGui/QSurfaceFormat>
#include <QtGui/QPainter>
#include <QtGui/QFont>

#include <iostream>

//#define USE_GLPAINTPIXELS
#undef USE_GLPAINTPIXELS

namespace BALL 
{
	namespace VIEW 
	{
	  // The old Qt4-era GL widget gave a 2.1 compatibility context by default;
	  // QOpenGLWidget does not. CompatibilityProfile + version 2.1 is required to
	  // keep glRenderer.C's ~100 fixed-function GL calls (glBegin/glMatrixMode/
	  // gluLookAt/...) working on macOS.
	  QSurfaceFormat GLRenderWindow::gl_format_ = [] {
				QSurfaceFormat fmt;
				fmt.setProfile(QSurfaceFormat::CompatibilityProfile);
				fmt.setOptions(QSurfaceFormat::DeprecatedFunctions); // Qt 6: explicit legacy entry points (D-06; Pitfall 2)
				fmt.setVersion(2, 1);                              // fixed-function pipeline
				fmt.setDepthBufferSize(24);                        // was DepthBuffer
				fmt.setStencilBufferSize(8);                       // was StencilBuffer
				fmt.setSwapBehavior(QSurfaceFormat::DoubleBuffer);  // was DoubleBuffer
				fmt.setSamples(4);                                 // was SampleBuffers
#ifndef BALL_OS_DARWIN
	/*
	 * Stereo buffers on Linux/X11 are broken in 5.7.0 and 5.7.1
	 * https://github.com/BALL-Project/ball/issues/630
	 */
#	if !defined(BALL_OS_LINUX) || QT_VERSION < QT_VERSION_CHECK(5, 7, 0) || QT_VERSION > QT_VERSION_CHECK(5, 7, 1)
				fmt.setStereo(true);                               // was StereoBuffers
#	endif
#endif
				// DirectRendering has no QSurfaceFormat equivalent — dropped (default behaviour)
				return fmt;
			}();


		GLRenderWindow::GLRenderWindow()
			: QOpenGLWidget(),
			  stereo_delta_(0.f),
			  m_screenTexID(0),
			  FB_TEXTURE_TARGET(GL_TEXTURE_2D),
			  FB_TEXTURE_FORMAT(GL_RGB),
			  FB_INTERNAL_TEXTURE_FORMAT(GL_RGB),
			  FB_TEXTURE_DATATYPE(GL_FLOAT),
			  ignore_events_(false),
			  down_sampling_factor_(1.f)
		{
			setFormat(gl_format_);          // must precede first show
			setAutoFillBackground(false);
			// QOpenGLWidget swaps buffers automatically after paintGL()
		}

		GLRenderWindow::GLRenderWindow(QWidget* parent_widget, const char* /*name*/, Qt::WindowFlags w_flags)
			: QOpenGLWidget(parent_widget, w_flags),
			  stereo_delta_(0.f),
			  m_screenTexID(0),
			  FB_TEXTURE_TARGET(GL_TEXTURE_2D),
			  FB_TEXTURE_FORMAT(GL_RGB),
			  FB_INTERNAL_TEXTURE_FORMAT(GL_RGB),
			  FB_TEXTURE_DATATYPE(GL_FLOAT),
			  ignore_events_(false),
			  down_sampling_factor_(1)
		{
			setFormat(gl_format_);          // must precede first show
			setAutoFillBackground(false);
			// QOpenGLWidget swaps buffers automatically after paintGL()
		}

		GLRenderWindow::GLRenderWindow(const GLRenderWindow& /*window*/, QWidget* parent_widget, const char* /*name*/, Qt::WindowFlags w_flags)
			: QOpenGLWidget(parent_widget, w_flags),
			  stereo_delta_(0.f),
			  m_screenTexID(0),
			  FB_TEXTURE_TARGET(GL_TEXTURE_2D),
			  FB_TEXTURE_FORMAT(GL_RGB),
			  FB_INTERNAL_TEXTURE_FORMAT(GL_RGB),
			  FB_TEXTURE_DATATYPE(GL_FLOAT),
			  ignore_events_(false),
			  down_sampling_factor_(1.f)
		{
			// Context sharing is established globally via Qt::AA_ShareOpenGLContexts
			// in BALLView main() — no per-widget share context constructor exists.
			setFormat(gl_format_);          // must precede first show
			setAutoFillBackground(false);
			// QOpenGLWidget swaps buffers automatically after paintGL()
		}

		GLRenderWindow::~GLRenderWindow()
		{
			deleteTexture();
		}

		void GLRenderWindow::initializeGL()
		{
			// QOpenGLWidget guarantees the context is current here, on the GUI thread.
#ifdef BALL_HAS_GLEW
			glewInit();
#endif
			checkGL();

			// DIAG-01: emit a single machine-parseable GL-capability line to stdout.
			// The context is current here, so glGetString() and the QOpenGLWidget
			// queries (format()/devicePixelRatioF()/defaultFramebufferObject()/size)
			// are all valid. This line is both a debugging aid and the render
			// smoke-check's oracle: it records exactly the fields that would have
			// caught the Phase 2 "renders blank because the GL context/FBO is wrong"
			// bugs (detached window, HiDPI viewport mismatch, paintGL never called).
			// It is unconditional (no debug flag), one physical line, std::cout (not
			// BALL::Log) so CI greps stdout, and flushed so it survives an early kill.
			{
				const char* gl_vendor   = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
				const char* gl_renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
				const char* gl_version  = reinterpret_cast<const char*>(glGetString(GL_VERSION));

				QSurfaceFormat fmt = format();   // the *actual* granted format
				const char* profile_str =
					(fmt.profile() == QSurfaceFormat::CoreProfile)          ? "core"          :
					(fmt.profile() == QSurfaceFormat::CompatibilityProfile) ? "compatibility" :
					                                                          "none";

				const qreal  dpr = devicePixelRatioF();
				const int    fbo_w = static_cast<int>(width()  * dpr);
				const int    fbo_h = static_cast<int>(height() * dpr);

				std::cout << "BALLVIEW_GL_DIAG"
				          << " gl_vendor=\""        << (gl_vendor   ? gl_vendor   : "unknown") << "\""
				          << " gl_renderer=\""      << (gl_renderer ? gl_renderer : "unknown") << "\""
				          << " gl_version=\""       << (gl_version  ? gl_version  : "unknown") << "\""
				          << " gl_profile="         << profile_str
				          << " surface_version="    << fmt.majorVersion() << "." << fmt.minorVersion()
				          << " depth_bits="         << fmt.depthBufferSize()
				          << " stencil_bits="       << fmt.stencilBufferSize()
				          << " samples="            << fmt.samples()
				          << " device_pixel_ratio=" << dpr
				          << " default_fbo="        << defaultFramebufferObject()
				          << " fbo_size="           << fbo_w << "x" << fbo_h
				          << " renderer_backend=GL"
				          << std::endl;
			}

			// QOpenGLWidget creates its GL context lazily on first show, unlike the
			// old QGLWidget which created it eagerly in its constructor. The owning
			// Scene therefore defers its GL-context-dependent init (render-setup
			// init, display-list allocation, texture setup) until now — the first
			// point at which a current context is guaranteed.
			if (Scene* scene = dynamic_cast<Scene*>(parentWidget()))
			{
				scene->initializeGLContext();
			}
		}

		void GLRenderWindow::resizeGL(int w, int h)
		{
			// The actual texture (re)allocation lives in GLRenderWindow::resize(),
			// a RenderWindow virtual driven by RenderSetup. Keep the GL viewport in
			// sync with the widget's framebuffer here; paintGL() sets its own
			// viewport for the texture blit.
			glViewport(0, 0, w, h);
		}

		void GLRenderWindow::paintGL()
		{
			// QOpenGLWidget guarantees: context current + defaultFramebufferObject()
			// bound. It swaps the buffers automatically when this returns — no manual
			// buffer-swap call is needed or possible.
			//
			// QOpenGLWidget's default framebuffer is only valid and current INSIDE
			// paintGL(). The QGLWidget-era code rendered the GL scene from an event
			// handler (Scene::handleRenderToBufferFinishedEvent_) into an FBO that
			// is undefined outside paintGL() — leaving the window empty until an
			// interaction forced a repaint, blank on resize, and (combined with the
			// stale refresh() texture blit) z-fighting/banding. All GL-renderer
			// framebuffer drawing now happens here instead.
			if (ignore_events_) return;

			// If a GLRenderer drives this window, render the molecular scene
			// directly into the (current, valid) default FBO. GLRenderer::
			// renderToBuffer() issues its own glClear(GL_COLOR_BUFFER_BIT |
			// GL_DEPTH_BUFFER_BIT) and depth-tested draw — exactly once per frame.
			if (render_setup_ != 0 &&
			    RTTI::isKindOf<GLRenderer>(render_setup_->renderer))
			{
				render_setup_->renderToBuffer();
				return;
			}

			// Otherwise (buffered/raytracer renderer): blit the CPU pixel buffer
			// produced by the worker thread as a fullscreen texture.
			refresh();
		}

		bool GLRenderWindow::init()
		{
			checkGL();

			// QSurfaceFormat has no rgba() query — RGBA is always implied.

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

			QOpenGLWidget::resize(width, height);

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
			float deltaRatio = 1.f - newRatio;

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

		void GLRenderWindow::beginFrame()
		{
			// GUI-thread-only: QOpenGLWidget's context + default FBO are GUI-thread-affine.
			// This holds the GL-context op moved out of RenderSetup::makeCurrent().
			if (QOpenGLContext::currentContext() != context())
				makeCurrent();   // QOpenGLWidget::makeCurrent -- the GL-context op
		}

		void GLRenderWindow::endFrame()
		{
			// QOpenGLWidget swaps implicitly after paintGL(); nothing to submit here.
			// Deliberate no-op for the GL backend; becomes meaningful for a QRhi backend.
		}

		void* GLRenderWindow::nativeHandle()
		{
			return static_cast<QOpenGLWidget*>(this);
		}

		void GLRenderWindow::renderText(int x, int y, const String& text, const ColorRGBA& color, Size size)
		{
			// The old Qt4 GL widget's renderText() is gone in QOpenGLWidget — draw
			// the on-screen text via a QPainter overlay on the widget instead (see
			// Scene::renderText_ for the same pattern). Must be called from within
			// paintGL()'s paint cycle, after the raw fixed-function GL was issued.
			QPainter painter(this);
			painter.setRenderHint(QPainter::TextAntialiasing, true);
			painter.setPen(QColor(color.getRed(),   color.getGreen(),
			                      color.getBlue(),  color.getAlpha()));

			QFont font;
			font.setPixelSize(size);
			font.setBold(true);
			painter.setFont(font);

			painter.drawText(x, y, QString::fromStdString(text));
			painter.end();
		}

		void GLRenderWindow::renderText(float /*x*/, float /*y*/, float /*z*/, const String& /*text*/, const ColorRGBA& /*color*/, Size /*size*/)
		{
			// 3D-coordinate overload was already a dead no-op before the port
			// (early return). Kept as a stub; world-space text would require a
			// manual gluProject + QPainter overlay if ever revived.
			return;
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
					// A worker thread finished a fresh CPU buffer — schedule a
					// repaint. paintGL() does the actual blit on the GUI thread and
					// QOpenGLWidget swaps automatically. No manual refresh()/swap here.
					update();
					break;
				default:
					break;
			}
		}

		void GLRenderWindow::lockGLContext()
		{
			// makeCurrent()/doneCurrent() on a QOpenGLWidget are GUI-thread-only:
			// its context and default FBO are GUI-thread-affine. Any caller of these
			// from a non-GUI thread is a bug — the worker (raytracer) is CPU-only and
			// must not touch GL. Callers are audited in Plans 03/04.
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
				float aperture = 60.f;
				float width = static_cast<float>(m_fmt.getWidth());
				
				//formula according to Paul Bourke
				//http://local.wasp.uwa.edu.au/~pbourke/miscellaneous/stereographics/stereorender/
				stereo_delta_ = (fabs(eye_separation) * width) / (focal_length * tan(Angle(aperture, false).toRadian())); 
				std::cout << stereo_delta_ << std::endl;
		}
	} // namespace VIEW
} //namespace BALL

