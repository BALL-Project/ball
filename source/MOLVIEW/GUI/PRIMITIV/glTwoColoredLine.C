// $Id: glTwoColoredLine.C,v 1.1 2000/09/23 15:39:16 hekl Exp $

#include <BALL/MOLVIEW/GUI/PRIMITIV/glTwoColoredLine.h>

using namespace std;

namespace BALL
{

	namespace MOLVIEW
	{

		GLTwoColoredLine::GLTwoColoredLine()
			:	TwoColoredLine(),
				GLObject()
		{
		}

		GLTwoColoredLine::GLTwoColoredLine
			(const GLTwoColoredLine& GL_two_colored_line, bool deep)
			:	TwoColoredLine(GL_two_colored_line, deep),
				GLObject(GL_two_colored_line, deep)
		{
		}

		GLTwoColoredLine::GLTwoColoredLine
			(const VIEW::GeometricObject& geometric_object)
			:	TwoColoredLine(geometric_object),
				GLObject()
		{
		}

		GLTwoColoredLine::~GLTwoColoredLine()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<GLTwoColoredLine>() << endl;
			#endif 

			destroy();
		}

		void GLTwoColoredLine::clear()
		{
			TwoColoredLine::clear();
			GLObject::clear();
		}

		void GLTwoColoredLine::destroy()
		{
			TwoColoredLine::destroy();
			GLObject::destroy();
		}

		bool GLTwoColoredLine::draw(bool with_names)
		{
			if (hasProperty(VIEW::GeometricObject::PROPERTY__OBJECT_HIDDEN) == true)
			{
				return true;
			}

			if (isSelected() == true)
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

			if (isSelected() == false)
			{
				glColor4ub((unsigned char)getColor1().red(),
									 (unsigned char)getColor1().green(),
									 (unsigned char)getColor1().blue(),
									 (unsigned char)getColor1().alpha());
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
				glColor4ub((unsigned char)getColor2().red(),
									 (unsigned char)getColor2().green(),
									 (unsigned char)getColor2().blue(),
									 (unsigned char)getColor2().alpha());
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
		{
			return TwoColoredLine::extract();
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/MOLVIEW/GUI/PRIMITIV/glTwoColoredLine.iC>
#		endif
 
	} // namespace MOLVIEW

} // namespace BALL
