// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: glpoint.C,v 1.6.2.1 2003/01/07 13:23:28 anker Exp $

#include <BALL/VIEW/GUI/PRIMITIV/glpoint.h>
#include <BALL/COMMON/rtti.h>

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
					<< " of class " << RTTI::getName<GLPoint>() << endl;
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
				glColor4ub((unsigned char)BALL_SELECTED_COLOR.getRed(),
									 (unsigned char)BALL_SELECTED_COLOR.getGreen(),
									 (unsigned char)BALL_SELECTED_COLOR.getBlue(),
									 (unsigned char)BALL_SELECTED_COLOR.getAlpha());
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

	} // namespace VIEW

} // namespace BALL
