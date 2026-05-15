// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_RENDERING_GLRENDERWINDOW_H
#define BALL_VIEW_RENDERING_GLRENDERWINDOW_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#ifndef BALL_DATATYPE_STRING_H
# include <BALL/DATATYPE/string.h>
#endif

#ifndef BALL_VIEW_RENDERING_RENDERWINDOW_H
# include <BALL/VIEW/RENDERING/renderWindow.h>
#endif

#ifndef BALL_VIEW_RENDERING_RENDERSURFACE_H
# include <BALL/VIEW/RENDERING/renderSurface.h>
#endif

#ifndef BALL_VIEW_DATATYPE_COLORRGBA_H
# include <BALL/VIEW/DATATYPE/colorRGBA.h>
#endif

#ifndef BALL_SYSTEM_MUTEX_H
# include <BALL/SYSTEM/mutex.h>
#endif

#include <QtOpenGLWidgets/QOpenGLWidget>
#include <QtGui/QSurfaceFormat>

namespace BALL
{
	namespace VIEW
	{
		class RenderSetup;

		/**
		 * Model of the \link RenderWindow \endlink which uses OpenGL to render its buffer to the screen
		 */
		class BALL_VIEW_EXPORT GLRenderWindow
			: public RenderWindow,
				public QOpenGLWidget
		{

		public:
			GLRenderWindow();
			// Qt 6: QFlags::QFlags(int) is no longer implicit, so `= 0` for a
			// QFlags-typed default arg is a hard error. Use the default-constructed
			// QFlags{} as the canonical "no flags" sentinel (binary-compatible with
			// the previous `= 0` initialisation).
			GLRenderWindow(QWidget* parent_widget, const char* name = NULL, Qt::WindowFlags w_flags = Qt::WindowFlags());
			GLRenderWindow(const GLRenderWindow& window, QWidget* parent_widget, const char* name = NULL, Qt::WindowFlags w_flags = Qt::WindowFlags());

			virtual ~GLRenderWindow();

			/* RenderWindow methods */
			virtual bool init();
			virtual bool resize(const unsigned int width, const unsigned int height);
			virtual void refresh();

			/* RenderSurface methods -- context-lifecycle verbs.
			 * beginFrame() holds the GL-context op that used to live inline in
			 * RenderSetup::makeCurrent(). endFrame() is a deliberate no-op for
			 * the GL backend (QOpenGLWidget swaps implicitly after paintGL()).
			 */
			virtual void beginFrame() override;
			virtual void endFrame() override;
			virtual void* nativeHandle() override;

			// render the given text in the given color and size at window coordinates (x, y)
			virtual void renderText(int x, int y, const String& text, const ColorRGBA& color, Size size = 16);
			// render the given text in the given color and size at world coordinates (x, y, z)
			virtual void renderText(float x, float y, float z, const String& text, const ColorRGBA& color, Size size = 16);

			/// Lock the context for the current thread and make it active
			void lockGLContext();

			/// Unlock the context for the current thread and make it active
			void unlockGLContext();

			/// Force the window to ignore paint events
			void ignoreEvents(bool ignore) {ignore_events_ = ignore;}

			/** Register the RenderSetup that drives this window.
			 *
			 *  When the active renderer is a GLRenderer, all framebuffer
			 *  rendering must happen inside paintGL() -- QOpenGLWidget only
			 *  guarantees a valid, current default FBO there. paintGL() uses
			 *  this back-reference to drive the GL renderer directly. For
			 *  buffered renderers (raytracer) it stays unused and paintGL()
			 *  falls back to the CPU-buffer texture blit (refresh()).
			 */
			void setRenderSetup(RenderSetup* rs) { render_setup_ = rs; }
			
			/// Set the window's downsampling factor. This is a speed up factor.
			void setDownsamplingFactor(float dsfactor) override
				{down_sampling_factor_ = dsfactor;}
			
			/// Set the stereo delta for raytracing in pixels.
			void setStereoDelta(float delta)
				{stereo_delta_ = delta;}
			
			void setupStereo(float eye_separation, float focal_length);

			/// Get the window's downsampling factor.
			float getDownsamplingFactor() const
				{return down_sampling_factor_;}

			float stereo_delta_;

			/** The shared GL surface format (compatibility profile, GL 2.1).
			 *  Public so BALLView's main() can install it as the default surface
			 *  format via QSurfaceFormat::setDefaultFormat() before QApplication.
			 */
			static QSurfaceFormat gl_format_;

		protected:

			/** This function handles custom Qt Events.
			 *  
			 *  The main use of this function is notification of a fresh buffer to
			 *  display from a separate renderer thread.
			 */
			virtual void customEvent(QEvent* evt);

			/* QOpenGLWidget overrides — GL work runs here on the GUI thread */
			virtual void initializeGL() override;
			virtual void resizeGL(int w, int h) override;
			virtual void paintGL() override;

			// ID of the fullscreen texture used to paste image into GPU framebuffer
			GLuint m_screenTexID;
			// type of the texture used
			GLenum FB_TEXTURE_TARGET;
			// format of the GL texture (GL_RGB, GL_RGBA, etc.)
			GLenum FB_TEXTURE_FORMAT;
			// internal format specified when creating the textures
			GLenum FB_INTERNAL_TEXTURE_FORMAT;
			// data type of the GL texture (GL_FLOAT, GL_UNSIGNED_INT, etc.)
			GLenum FB_TEXTURE_DATATYPE;
					
			void createTexture(const unsigned int winWidth, const unsigned int winHeight);	
			void deleteTexture();

			void checkGL();

			bool errorInGL(GLenum& error);
			String getGLErrorString(GLenum error);

			mutable Mutex contex_mutex_;
			bool ignore_events_;
			float down_sampling_factor_;
			//float stereo_delta_;

			// The RenderSetup driving this window (see setRenderSetup()).
			// Non-owning; null until a RenderSetup registers itself.
			RenderSetup* render_setup_ = nullptr;
		};

	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_RENDERING_GLRENDERWINDOW_H
