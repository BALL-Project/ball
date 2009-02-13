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

#include <QtOpenGL/qgl.h>

namespace BALL
{
	namespace VIEW
	{				
		/**
		 * Model of the \link RenderWindow \endlink which uses OpenGL to render its buffer to the screen
		 */
		class GLRenderWindow 
			: public RenderWindow<float>,
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

			void setRenderText(const QPoint& position, const String& text, const ColorRGBA& color)
			{
				info_point_ = position;
				info_text_  = text;
				info_color_ = color;
			}

		protected:	
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

			// position and text of possible information texts like fps
			QPoint 	  info_point_;
			String 	  info_text_;
			ColorRGBA info_color_;
		};

	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_RENDERING_GLRENDERWINDOW_H
