// $Id: glTwoColoredLine.C,v 1.5 2001/05/13 15:02:41 hekl Exp $

#include <BALL/MOLVIEW/GUI/PRIMITIV/glTwoColoredLine.h>

using namespace std;

namespace BALL
{

	namespace MOLVIEW
	{

		GLTwoColoredLine::GLTwoColoredLine()
			throw()
			:	TwoColoredLine(),
				GLObject()
		{
		}

		GLTwoColoredLine::GLTwoColoredLine
			(const GLTwoColoredLine& GL_two_colored_line, bool deep)
			throw()
			:	TwoColoredLine(GL_two_colored_line, deep),
				GLObject(GL_two_colored_line)
		{
		}

		GLTwoColoredLine::GLTwoColoredLine
			(const VIEW::GeometricObject& geometric_object)
			throw()
			:	TwoColoredLine(geometric_object),
				GLObject()
		{
		}

		GLTwoColoredLine::~GLTwoColoredLine()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<GLTwoColoredLine>() << endl;
			#endif 

			destroy();
		}

		void GLTwoColoredLine::clear()
			throw()
		{
			TwoColoredLine::clear();
			GLObject::clear();
		}

		void GLTwoColoredLine::destroy()
			throw()
		{
			TwoColoredLine::destroy();
			GLObject::destroy();
		}

		void GLTwoColoredLine::set(const GLTwoColoredLine& gl_two_colored_line, bool deep)
			throw()
		{
			TwoColoredLine::set(gl_two_colored_line, deep);
			GLObject::set(gl_two_colored_line);
		}

		const GLTwoColoredLine& GLTwoColoredLine::operator = (const GLTwoColoredLine& gl_two_colored_line)
			throw()
		{
			set(gl_two_colored_line);

			return *this;
		}

		void GLTwoColoredLine::get(GLTwoColoredLine& gl_two_colored_line, bool deep) const
			throw()
		{
			gl_two_colored_line.set(*this, deep);
		}

		void GLTwoColoredLine::swap(GLTwoColoredLine& gl_two_colored_line)
			throw()
		{
			TwoColoredLine::swap(gl_two_colored_line);
			GLObject::swap(gl_two_colored_line);
		}

		bool GLTwoColoredLine::draw(bool with_names)
			throw()
		{
			if (hasProperty(VIEW::GeometricObject::PROPERTY__OBJECT_HIDDEN))
			{
				return true;
			}

			if (isSelected())
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

			if (isSelected() == false)
			{
				glColor4ub((unsigned char)getColor1().getRed(),
									 (unsigned char)getColor1().getGreen(),
									 (unsigned char)getColor1().getBlue(),
									 (unsigned char)getColor1().getAlpha());
			}

			glBegin(GL_LINE_STRIP);
			glVertex3f((GLfloat)(getVertex1().x),
								 (GLfloat)(getVertex1().y),
								 (GLfloat)(getVertex1().z));
			glVertex3f((GLfloat)(getMiddleVertex().x),
								 (GLfloat)(getMiddleVertex().y),
								 (GLfloat)(getMiddleVertex().z));

			if (isSelected() == false)
			{
				glColor4ub((unsigned char)getColor2().getRed(),
									 (unsigned char)getColor2().getGreen(),
									 (unsigned char)getColor2().getBlue(),
									 (unsigned char)getColor2().getAlpha());
			}

			glVertex3f((GLfloat)(getMiddleVertex().x),
								 (GLfloat)(getMiddleVertex().y),
								 (GLfloat)(getMiddleVertex().z));
			glVertex3f((GLfloat)(getVertex2().x),
								 (GLfloat)(getVertex2().y),
								 (GLfloat)(getVertex2().z));
			glEnd();

			return true;
		}

		bool GLTwoColoredLine::extract()
			throw()
		{
			return TwoColoredLine::extract();
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/MOLVIEW/GUI/PRIMITIV/glTwoColoredLine.iC>
#		endif
 
	} // namespace MOLVIEW

} // namespace BALL
