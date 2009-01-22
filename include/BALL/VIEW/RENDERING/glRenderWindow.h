// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_RENDERING_GLRENDERWINDOW_H
#define BALL_VIEW_RENDERING_GLRENDERWINDOW_H

#include <BALL/COMMON/global.h>
#include <BALL/DATATYPE/string.h>
#include <BALL/VIEW/RENDERING/renderWindow.h>

namespace BALL
{
	namespace VIEW
	{				
        /**
         * Model of the \link RenderWindow \endlink which uses OpenGL to render its buffer to the screen
         * This is pure OpenGL window, e.g. it does not solve the opening and closing of a real window.
         * It assumes an appropriate OpenGL context is active when its methods are called.
         * A window which also opens/closes system window and manages OpenGL context should be implemented
         * as a descendat of this GLRenderWindow.
         */
		class GLRenderWindow : public RenderWindow<float>
		{
			
		public:
			GLRenderWindow();
			virtual ~GLRenderWindow();

			/* RenderWindow methods */
			virtual bool init();
			virtual bool resize(const unsigned int width, const unsigned int height);
			virtual void refresh();			
			
		private:	

			GLuint m_screenTexID;					// ID of the fullscreen texture used to paste image into GPU framebuffer            
            GLenum FB_TEXTURE_TARGET;               // type of the texture used
			GLenum FB_TEXTURE_FORMAT;		        // format of the GL texture (GL_RGB, GL_RGBA, etc.)
            GLenum FB_INTERNAL_TEXTURE_FORMAT;      // internal format specified when creating the textures
			GLenum FB_TEXTURE_DATATYPE;	            // data type of the GL texture (GL_FLOAT, GL_UNSIGNED_INT, etc.						
					
			void createTexture(const unsigned int winWidth, const unsigned int winHeight);	
			void deleteTexture();

            void checkGL();

            bool errorInGL(GLenum& error);
            String getGLErrorString(GLenum error);
		};

	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_RENDERING_GLRENDERWINDOW_H
