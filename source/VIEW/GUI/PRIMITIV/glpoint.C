// $Id: glpoint.C,v 1.5 2001/02/12 12:02:28 amoll Exp $

#include <BALL/VIEW/GUI/PRIMITIV/glpoint.h>
#include <GL/gl.h>

namespace BALL
{

	namespace VIEW
	{


		GLPoint::GLPoint()
			:	Point(),
				GLObject()
		{
		}

		GLPoint::GLPoint(const GLPoint& GL_point, bool deep)
			:	Point(GL_point, deep),
				GLObject(GL_point)
		{
		}

		GLPoint::GLPoint(const GeometricObject& geometric_object)
			:	Point(geometric_object),
				GLObject()
		{
		}

		GLPoint::~GLPoint()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTIT<GLPoint>::getName() << endl;
			#endif 

			destroy();
		}

		void GLPoint::clear()
			throw()
		{
			Point::clear();
			GLObject::clear();
		}

		void GLPoint::destroy()
			throw()
		{
			Point::destroy();
			GLObject::destroy();
		}

		bool GLPoint::draw(bool with_names)
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
			
			glBegin(GL_POINTS);
			glVertex3f((GLfloat)(getVertex().x),
								 (GLfloat)(getVertex().y),
								 (GLfloat)(getVertex().z));
			glEnd();
			
			glPopMatrix();

			return true;
		}

		bool GLPoint::extract()
			throw()
		{
			return Point::extract();
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/GUI/PRIMITIV/glpoint.iC>
#		endif

	} // namespace VIEW

} // namespace BALL
