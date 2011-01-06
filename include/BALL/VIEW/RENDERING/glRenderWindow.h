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

#ifndef BALL_VIEW_DATATYPE_COLORRGBA_H
# include <BALL/VIEW/DATATYPE/colorRGBA.h>
#endif

#ifndef BALL_SYSTEM_MUTEX_H
# include <BALL/SYSTEM/mutex.h>
#endif

#include <QtOpenGL/qgl.h>

namespace BALL
{
	namespace VIEW
	{				
		/**
		 * Model of the \link RenderWindow \endlink which uses OpenGL to render its buffer to the screen
		 */
		class BALL_VIEW_EXPORT GLRenderWindow 
			: public RenderWindow,
				public QGLWidget
		{
			
		public:
			GLRenderWindow();
			GLRenderWindow(QWidget* parent_widget, const char* name = NULL, Qt::WFlags w_flags = 0);
			GLRenderWindow(const GLRenderWindow& window, QWidget* parent_widget, const char* name = NULL, Qt::WFlags w_flags = 0);

			virtual ~GLRenderWindow();

			/* RenderWindow methods */
			virtual bool init();
			virtual bool resize(const unsigned int width, const unsigned int height);
			virtual void refresh();			

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
			
			/// Set the window's downsampling factor. This is a speed up factor.
			void setDownsamplingFactor(float dsfactor)
				{down_sampling_factor_ = dsfactor;}
			
			/// Set the stereo delta for raytracing in pixels.
			void setStereoDelta(float delta)
				{stereo_delta_ = delta;}
			
			void setupStereo(float eye_separation, float focal_length);
			
			/// Get the window's downsampling factor.
			float getDownsamplingFactor() const
				{return down_sampling_factor_;}
			
			float stereo_delta_;

		protected:	

			/** This function handles custom Qt Events.
			 *  
			 *  The main use of this function is notification of a fresh buffer to
			 *  display from a separate renderer thread.
			 */
			virtual void customEvent(QEvent* evt);

			void paintEvent(QPaintEvent* e);
			static QGLFormat gl_format_;

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
		};

	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_RENDERING_GLRENDERWINDOW_H
