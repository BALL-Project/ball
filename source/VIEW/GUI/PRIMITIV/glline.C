// $Id: glline.C,v 1.3 2000/12/22 19:12:17 amoll Exp $

#include <BALL/VIEW/GUI/PRIMITIV/glline.h>
#include <GL/gl.h>

namespace BALL
{

	namespace VIEW
	{

		GLLine::GLLine()
			throw()
			:	Line(),
				GLObject()
		{
		}

		GLLine::GLLine(const GLLine& GL_line, bool deep)
			throw()
			:	Line(GL_line, deep),
				GLObject(GL_line, deep)
		{
		}

		GLLine::GLLine(const GeometricObject& geometric_object)
			:	Line(geometric_object),
				GLObject()
		{
		}

		GLLine::~GLLine()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<GLLine>() << endl;
			#endif 

			destroy();
		}

		void GLLine::clear()
			throw()
		{
			Line::clear();
			GLObject::clear();
		}

		void GLLine::destroy()
			throw()
		{
			Line::destroy();
			GLObject::destroy();
		}

		bool GLLine::draw(bool with_names)
		{
			if (hasProperty(GeometricObject::PROPERTY__OBJECT_HIDDEN) == true)
			{
				return true;
			}

			if (isSelected() == false)
			{
				glColor4ub((unsigned char)getColor().red(),
									 (unsigned char)getColor().green(),
									 (unsigned char)getColor().blue(),
									 (unsigned char)getColor().alpha());
			}
			else
			{
				glColor4ub((unsigned char)getSelectedColor().red(),
									 (unsigned char)getSelectedColor().green(),
									 (unsigned char)getSelectedColor().blue(),
									 (unsigned char)getSelectedColor().alpha());
			}

			if (with_names)
			{
				glLoadName(getGLPrimitiveManager()->getName(*this));
			}

			glPushMatrix();
			
			// drawing mode dots must be implemented
			//  int precision;
			//  int mode;

			//  getDrawingPrecisionAndMode(precision, mode);

			glBegin(GL_LINES);
			glVertex3f((GLfloat)(getVertex1().x),
								 (GLfloat)(getVertex1().y),
								 (GLfloat)(getVertex1().z));
			glVertex3f((GLfloat)(getVertex2().x),
								 (GLfloat)(getVertex2().y),
								 (GLfloat)(getVertex2().z));
			glEnd();

			
			glPopMatrix();

			return true;
		}

		bool GLLine::extract()
			throw()
		{
			return Line::extract();
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/GUI/PRIMITIV/glline.iC>
#		endif
		
	} // namespace VIEW

} // namespace BALL
