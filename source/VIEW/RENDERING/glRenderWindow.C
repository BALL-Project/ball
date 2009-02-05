// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

# include <GL/glew.h>
#include <BALL/VIEW/RENDERING/glRenderWindow.h>
#include <BALL/COMMON/logStream.h>

namespace BALL 
{
	namespace VIEW 
	{

		GLRenderWindow::GLRenderWindow()
			: FB_TEXTURE_FORMAT(GL_RGB), 
			  FB_TEXTURE_DATATYPE(GL_FLOAT),
              FB_TEXTURE_TARGET(GL_TEXTURE_2D),
              FB_INTERNAL_TEXTURE_FORMAT(GL_RGB)
		{		
			m_screenTexID = 0;
		}

		GLRenderWindow::~GLRenderWindow()
		{
			deleteTexture();
		}

		bool GLRenderWindow::init()
		{
			checkGL();

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
			if(!RenderWindow::resize(width, height))
			{
				return false;
			}
			createTexture(width, height);
			return true;
		}						

		void GLRenderWindow::refresh()
		{			
            RenderWindow::refresh();

			glPushAttrib(GL_TEXTURE_BIT);
			glPushAttrib(GL_DEPTH_BUFFER_BIT);

				glBindTexture(FB_TEXTURE_TARGET, m_screenTexID);
				glTexSubImage2D(FB_TEXTURE_TARGET, 0, 0, 0, m_fmt.getWidth(), m_fmt.getHeight(), 
								FB_TEXTURE_FORMAT, FB_TEXTURE_DATATYPE, m_pixels.get());                
					
				glEnable(FB_TEXTURE_TARGET);
				glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
				
				glDisable(GL_DEPTH_TEST);
				glMatrixMode(GL_PROJECTION);
				glPushMatrix();
					
					glLoadIdentity();
					
					glPushAttrib(GL_VIEWPORT_BIT);
						glViewport(0, 0, m_fmt.getWidth(), m_fmt.getHeight());
						float aspectRatio = static_cast<float>(m_fmt.getWidth()) / m_fmt.getHeight();
						glOrtho(-aspectRatio, aspectRatio, -1.0f, 1.0f, -1.0f, 1.0f);
						
						glBegin(GL_QUADS);
						
							glTexCoord2f(1.0f, 1.0f);
							glVertex2f(-aspectRatio, -1.0f);
							
							glTexCoord2f(0.0f, 1.0f );
							glVertex2f(aspectRatio, -1.0f);	
							
							glTexCoord2f(0.0f, 0.0f );
							glVertex2f(aspectRatio, 1.0f);	
							
							glTexCoord2f(1.0f, 0.0f);
							glVertex2f(-aspectRatio, 1.0f);
					
							glEnd();
					glPopAttrib();
				
				glPopMatrix();	
				glMatrixMode(GL_MODELVIEW);				

			glPopAttrib();
			glPopAttrib();
		}

		void GLRenderWindow::createTexture(const unsigned int width, const unsigned int height)
		{
			if(m_screenTexID != 0)
			{
				deleteTexture();
			}

			glGenTextures(1, &m_screenTexID);

			glBindTexture(FB_TEXTURE_TARGET, m_screenTexID);			                

				glTexParameteri(FB_TEXTURE_TARGET, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(FB_TEXTURE_TARGET, GL_TEXTURE_MAG_FILTER, GL_NEAREST);                
						
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

	} // namespace VIEW
} //namespace BALL

