// $Id: glpoint.C,v 1.6 2001/05/13 14:28:37 hekl Exp $

#include <BALL/VIEW/GUI/PRIMITIV/glpoint.h>
#include <GL/gl.h>

namespace BALL
{

	namespace VIEW
	{


		GLPoint::GLPoint()
			throw()
			:	Point(),
				GLObject()
		{
		}

		GLPoint::GLPoint(const GLPoint& GL_point, bool deep)
			throw()
			:	Point(GL_point, deep),
				GLObject(GL_point)
		{
		}

		GLPoint::GLPoint(const GeometricObject& geometric_object)
			throw()
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

		void GLPoint::set(const GLPoint& point, bool deep)
			throw()
		{
			Point::set(point, deep);
			GLObject::set(point);
		}

		const GLPoint& GLPoint::operator = (const GLPoint& point)
			throw()
		{
			set(point);
			return *this;
		}

		void GLPoint::get(GLPoint& point, bool deep) const
			throw()
		{
			point.set(*this, deep);
		}

		void GLPoint::swap(GLPoint& point)
			throw()
		{
			Point::swap(point);
			GLObject::swap(point);
		}

		bool GLPoint::draw(bool with_names)
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
