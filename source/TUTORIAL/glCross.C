// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: glCross.C,v 1.2 2002/02/27 12:25:06 sturm Exp $
// molview tutorial example
// ------------------------
// create a geometric primitive

#include "glCross.h"

namespace BALL
{

	namespace VIEW
	{

		GLCross::GLCross()
			throw()
		{
		}
		
		GLCross::~GLCross()
			throw()
		{
		}

		bool GLCross::draw(bool with_names)
			throw()
		{

			// if the property of the object is set to PROPERTY__OBJECT_HIDDEN, do
			// not draw it
			if (hasProperty(GeometricObject::PROPERTY__OBJECT_HIDDEN))
			{
				return true;
			}

			// if it is selectd, use the appropriate color, else use the one
			// assigned by the method setColor()
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

			// if we want the object to be picked, we need a name by which it can be
			// identified
			if (with_names)
			{
				glLoadName(getGLPrimitiveManager()->getName(*this));
			}

			// render the cross
			glBegin(GL_LINES);
			glVertex3f((GLfloat)(getVertex().x - getRadius()),
								 (GLfloat)(getVertex().y),
								 (GLfloat)(getVertex().z));
			glVertex3f((GLfloat)(getVertex().x + getRadius()),
								 (GLfloat)(getVertex().y),
								 (GLfloat)(getVertex().z));

			glVertex3f((GLfloat)(getVertex().x),
								 (GLfloat)(getVertex().y - getRadius()),
								 (GLfloat)(getVertex().z));
			glVertex3f((GLfloat)(getVertex().x),
								 (GLfloat)(getVertex().y + getRadius()),
								 (GLfloat)(getVertex().z));

			glVertex3f((GLfloat)(getVertex().x),
								 (GLfloat)(getVertex().y),
								 (GLfloat)(getVertex().z - getRadius()));
			glVertex3f((GLfloat)(getVertex().x),
								 (GLfloat)(getVertex().y),
								 (GLfloat)(getVertex().z + getRadius()));
			glEnd();

			return true;
		}

	}

}

