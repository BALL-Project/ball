// $Id: glline.C,v 1.5 2001/05/13 14:28:37 hekl Exp $

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
				GLObject(GL_line)
		{
		}

		GLLine::GLLine(const GeometricObject& geometric_object)
			throw()
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

		void GLLine::set(const GLLine& line, bool deep)
			throw()
		{
			Line::set(line, deep);
			GLObject::set(line);
		}

		const GLLine& GLLine::operator = (const GLLine& line)
			throw()
		{
			set(line);
			return *this;
		}

		void GLLine::get(GLLine& line, bool deep) const
			throw()
		{
			line.set(*this, deep);
		}

		void GLLine::swap(GLLine& line)
			throw()
		{
			Line::swap(line);
			GLObject::swap(line);
		}

		bool GLLine::draw(bool with_names)
			throw()
		{
			if (hasProperty(GeometricObject::PROPERTY__OBJECT_HIDDEN) == true)
			{
				return true;
			}

			if (isSelected() == false)
			{
				glColor4ub((unsigned char)getColor().getRed(),
									 (unsigned char)getColor().getGreen(),
									 (unsigned char)getColor().getBlue(),
									 (unsigned char)getColor().getAlpha());
			}
			else
			{
				glColor4ub((unsigned char)getSelectedColor().getRed(),
									 (unsigned char)getSelectedColor().getGreen(),
									 (unsigned char)getSelectedColor().getBlue(),
									 (unsigned char)getSelectedColor().getAlpha());
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
